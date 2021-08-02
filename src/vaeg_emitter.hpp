#ifndef VAEG_EMITTER
#define VAEG_EMITTER

#include "./vaeg_gdnative.hpp"
#include "../VAE/src/core/device/vae_rtaudio.hpp"
#include "../VAE/src/core/vae_config.hpp"
#include "../VAE/external/tklb/src/types/THeapBuffer.hpp"
#include "../VAE/external/tklb/src/types/TSpinLock.hpp"

#include <string.h>
#include <stdio.h>

namespace vaeg {
	struct Emitter;

	class MixerShared {
		using Mutex = tklb::SpinLock;
		using Emitters = tklb::HeapBuffer<Emitter*>;
		Emitters emitters;
		Mutex mutex;

	public:

		void add(Emitter* e) {
			Mutex::Lock lock(mutex);
			emitters.push(e);
		}

		void remove(Emitter* e) {
			Mutex::Lock lock(mutex);
			emitters.remove(e);
		}

		void lock() {
			mutex.lock();
		}

		void unlock() {
			mutex.unlock();
		}

		Emitters& get_emitters() {
			return emitters;
		}
	};

	MixerShared mixerShared;
	godot_method_bind* get_buffer_data_bind;
	godot_method_bind* get_rid_bind;
	godot_method_bind* get_id_bind;

	struct Emitter {
		tklb::AudioBufferFloat buffer;
		bool playing = false;
		bool loop = false;
		size_t time = 0;
		int rid = 0;
		godot_variant sample;

		VAEG_CLASS(Emitter)
		VAEG_SETGET(Emitter, playing, bool)
		VAEG_SETGET(Emitter, loop, bool)
		// VAEG_SETGET(Emitter, sample_ref, object)

		static void _register(void* handle) {
			VAEG_REGISTER_PROP(handle, Emitter, playing, bool, false)
			VAEG_REGISTER_PROP(handle, Emitter, loop, bool, false)
			VAEG_REGISTER_PROP_HINT(
				handle, Emitter, sample_ref, object, nullptr,
				GODOT_PROPERTY_HINT_RESOURCE_TYPE, "AudioStreamSample"
			)
			VAEG_REGISTER_METHOD(handle, Emitter, do_some)

			get_buffer_data_bind = api->godot_method_bind_get_method("AudioStreamSample", "get_data");
			get_rid_bind = api->godot_method_bind_get_method("Resource", "get_rid");
			get_id_bind = api->godot_method_bind_get_method("RID", "get_id");
			// VAEG_REGISTER_METHOD(Emitter, set_stream)
		}

		VAEG_FUNC(Emitter, do_some) {
			godot_variant ret;
			api->godot_variant_new_nil(&ret);
			return ret;
		}

		VAEG_SET(Emitter, sample_ref, object) {
			// godot_rid rid_object;
			// api->godot_rid_new(&rid_object);
			// api->godot_method_bind_ptrcall(get_rid_bind, value, no_args, &rid_object);
			// int new_rid;
			// api->godot_method_bind_ptrcall(get_id_bind, &rid_object, no_args, &new_rid);
			// if (new_rid == emitter->rid) {
			// 	return;
			// }

			api->godot_variant_new_object(&sample, parameter);

			godot_pool_byte_array pool;
			api->godot_pool_byte_array_new(&pool);
			const void* no_args[1] = { };
			api->godot_method_bind_ptrcall(get_buffer_data_bind, parameter, no_args, &pool);
			godot_int size = api->godot_pool_byte_array_size(&pool);
			const int channels = 2;
			const int length = size / (channels * 2);
			godot_pool_byte_array_read_access* read_access = api->godot_pool_byte_array_read(&pool);
			const uint8_t* data = api->godot_pool_byte_array_read_access_ptr(read_access);

			mixerShared.remove(this);
			buffer.resize(length, 2);
			buffer.setFromInterleaved(reinterpret_cast<const short*>(data), length, channels);
			buffer.multiply(0.0001f);
			mixerShared.add(this);

			api->godot_pool_byte_array_read_access_destroy(read_access);
			api->godot_pool_byte_array_destroy(&pool);
		}

		VAEG_GET(Emitter, sample_ref) {
			return sample;
		}

		Emitter() { }

		~Emitter() {
			api->godot_variant_destroy(&sample);
			mixerShared.remove(this);
		}
	};

	struct Mixer {
		using Backend = vae::core::BackendRtAudio;
		using Device = vae::core::DeviceRtaudio;

		Device* device = nullptr;
		size_t time = 0;
		bool running = true;
		bool threadExited = false;
		Mixer() {
			return;
			auto& backend = Backend::instance();

			vae::core::Device::SyncCallback callback =
			[&](const vae::core::Device::AudioBuffer& fromDevice, vae::core::Device::AudioBuffer& toDevice) {
				if (!running) {
					threadExited = true;
					return;
				}
				toDevice.set(0.0f);
				const size_t samples = toDevice.validSize();
				mixerShared.lock();
				auto& emitters = mixerShared.get_emitters();
				const int count =  emitters.size();
				for (int index = 0; index < count; index++) {
					auto& i = emitters[index];
					if (!i->playing) { continue; }
					const size_t startTime = i->time;
					const auto& buffer = i->buffer;
					const size_t totalLength = buffer.size();
					const size_t length = std::min(samples, totalLength - startTime);
					for (int c = 0; c < toDevice.channels(); c++) {
						int channel = c % buffer.channels();
						for (size_t s = 0; s < length; s++) {
							toDevice[c][s] += buffer[c][startTime + s];
						}
					}
					if (totalLength <= startTime + length) {
						i->time = 0;;
						if (!i->loop) {
							// stop if not looping
							i->playing = false;
						}
					} else {
						i->time = startTime + length;
					}
				}
				mixerShared.unlock();

				time += samples;
			};
			device = new Device(callback); // sync output device
			device->openDevice();
		}

		~Mixer() {
			// running = false;
			// while (!threadExited)
			// {
			// }
			// // device->closeDevice();
			// delete device;
		}
	};

	Mixer MainMixer;
}

#endif // VAEG_EMITTER

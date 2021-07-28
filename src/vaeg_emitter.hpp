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
	class Emitter;

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

	struct Emitter {
		tklb::AudioBufferFloat buffer;
		bool playing = false;
		bool loop = false;
		size_t time = 0;
		godot_object* sample_ref;

		VAEG_CLASS(Emitter)
		VAEG_SETGET(Emitter, playing, bool)
		VAEG_SETGET(Emitter, loop, bool)
		// VAEG_SETGET(Emitter, sample_ref, object)

		static void _register(void* handle) {
			VAEG_REGISTER_PROP(handle, Emitter, playing, bool, false)
			VAEG_REGISTER_PROP(handle, Emitter, loop, bool, false)
			VAEG_REGISTER_PROP(handle, Emitter, sample_ref, object, nullptr)

			get_buffer_data_bind = api->godot_method_bind_get_method("AudioStreamSample", "get_data");
			// VAEG_REGISTER_METHOD(Emitter, set_stream)
		}

		//static inline Object *___get_from_variant(Variant a) { godot_object *o = (godot_object*) a; return (o) ? (Object *) godot::nativescript_1_1_api->godot_nativescript_get_instance_binding_data(godot::_RegisterState::language_index, o) : nullptr; }

		static void _set_sample_ref(
			godot_object* instance_pointer, void* method_data,
			void* user_data, godot_variant* value
		) {
			auto emitter = reinterpret_cast<Emitter*>(user_data);
			emitter->sample_ref = vaeg::api->godot_variant_as_object(value);

			if (!emitter->sample_ref) {
				mixerShared.remove(emitter);
				emitter->buffer.resize(0);
				return;
			}

			godot_pool_byte_array pool;
			const void* no_args[1] = { };
			api->godot_method_bind_ptrcall(get_buffer_data_bind, emitter->sample_ref, no_args, &pool);
			godot_int size = api->godot_pool_byte_array_size(&pool);
			const int channels = 2;
			const int length = size / (channels * 2);
			godot_pool_byte_array_read_access* read_access = api->godot_pool_byte_array_read(&pool);
			const uint8_t* data = api->godot_pool_byte_array_read_access_ptr(read_access);
			mixerShared.remove(emitter);
			emitter->buffer.resize(length, 2);
			emitter->buffer.setFromInterleaved(reinterpret_cast<const short*>(data), length, channels);
			emitter->buffer.multiply(0.0001);
			mixerShared.add(emitter);
			api->godot_pool_byte_array_read_access_destroy(read_access);
			api->godot_pool_byte_array_destroy(&pool);
		}
		static godot_variant _get_sample_ref(
			godot_object* instance_pointer, void* method_data, void* user_data
		) {
			auto instance = reinterpret_cast<Emitter*>(user_data);
			godot_variant ret;
			vaeg::api->godot_variant_new_object(&ret, instance->sample_ref);
			return ret;
		}

		Emitter() { }

		~Emitter() {
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
			running = false;
			while (!threadExited)
			{
			}
			device->closeDevice();
			delete device;
		}
	};

	Mixer MainMixer;
}

#endif // VAEG_EMITTER

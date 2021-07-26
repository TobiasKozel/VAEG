#ifndef VAEG_EMITTER
#define VAEG_EMITTER

#include <Godot.hpp>
#include <Spatial.hpp>
#include <AudioStreamSample.hpp>

#include "../VAE/src/core/device/vae_portaudio.hpp"
#include "../VAE/src/core/vae_config.hpp"
#include "../VAE/external/tklb/src/types/THeapBuffer.hpp"

namespace godot { namespace vaeg {
	class Emitter;

	class MixerShared {
		MixerShared() { }
	public:
		tklb::HeapBuffer<Emitter*> emitters;
		std::function<void(Emitter*)> regFun;
		std::function<void(Emitter*)> relFun;

		static MixerShared& instance() {
			static MixerShared shared;
			return shared;
		}

	};
} }


namespace godot { namespace vaeg {
	class Emitter : public Spatial {
		GODOT_CLASS(Emitter, Spatial)
	private:
		size_t mTime = 0;
		Ref<AudioStreamSample> mStream;
		PoolByteArray mBytes;
	public:

		~Emitter() {
			MixerShared::instance().emitters.remove(this);
			Godot::print("Remove audio instance");
		}

		bool mPlaying = false;
		bool mLoop = false;

		static void _register_methods() {
			// register_method("_process", &Vaeg::_process);
			register_property<Emitter, Ref<AudioStreamSample>>(
				"stream",
				&Emitter::set_stream,
				&Emitter::get_stream,
				Ref<AudioStreamSample>()
			);

			register_property<Emitter, bool>("playing", &Emitter::mPlaying, false);
			register_property<Emitter, bool>("loop", &Emitter::mLoop, false);
		}

		void _init() {
			Godot::print("Push audio instance");
			printf("test push audio\n");
			MixerShared::instance().emitters.push(this);
		}

		void set_stream(Ref<AudioStreamSample> s) {
			mTime = 0;
			mStream = s;
			if (s.is_valid()) {
				if (s->get_mix_rate() != vae::Config::SampleRate) {
					Godot::print("Mixrate missmatch!");
				}

				if (s->get_format() != 1) {
					Godot::print("Unknown format!");
				}
				mBytes = mStream->get_data();
			}
		}

		size_t get_time() const {
			return mTime;
		}

		void set_time(const size_t&& time) {
			mTime = time;
		}

		size_t get_length() const {
			if (mStream.is_valid()) { return 0; }
			return mStream->get_length();
		}

		int get_channels() {
			if (mStream.is_valid()) { return 0; }
			return mStream->is_stereo() ? 2 : 1;
		}

		Ref<AudioStreamSample> get_stream() { return mStream; }

		const int16_t* get_data() {
			if (!mStream.is_valid()) { return nullptr; }
			return reinterpret_cast<const int16_t*>(mBytes.read().ptr());
		}
	};

	struct Mixer {
		Mixer() {
			Godot::print("Create Mixer");
			printf("test mixer created\n");
			vae::DeviceInfo devices[100];
			auto& backend = vae::core::BackendPortAudio::instance();
			for (unsigned int i = 0; i < backend.getDeviceCount(); i++) {
				devices[i] = backend.getDevice(i);
			}

			size_t time = 0;
			vae::core::Device::SyncCallback callback =
			[&](const vae::core::Device::AudioBuffer& fromDevice, vae::core::Device::AudioBuffer& toDevice) {
				const size_t samples = toDevice.validSize();
				for (int index = 0; MixerShared::instance().emitters.size(); index++) {
					auto& i = MixerShared::instance().emitters[index];
					if (!i->mPlaying) { continue; }
					const size_t startTime = i->get_time();
					const size_t totalLength = i->get_length();
					const size_t length = std::min(samples, i->get_length() - startTime);
					// for (int c = 0; c < toDevice.channels(); c++) {
					// 	int channel = c % i->get_channels();
					// 	const int16_t* buf = i->get_data();
					// 	for (size_t s = 0; s < length; s++) {
					// 		toDevice[c][s] += buf[startTime + s] / 32767.0;
					// 	}
					// }
					if (totalLength <= startTime + length) {
						i->set_time(0);
						if (!i->mLoop) {
							// stop if not looping
							i->mPlaying = false;
						}
					} else {
						i->set_time(startTime + length);
					}
				}

				time += samples;
			};
			vae::core::DevicePortaudio outputDevice(callback); // sync output device
			outputDevice.openDevice();
		}
	};
} } // godot::vaeg



#endif // VAEG_EMITTER

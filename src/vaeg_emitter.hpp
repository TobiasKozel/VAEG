#ifndef VAEG_EMITTER
#define VAEG_EMITTER

#include <Godot.hpp>
#include <Spatial.hpp>
#include <AudioStreamSample.hpp>

#include "../VAE/src/core/device/vae_rtaudio.hpp"
#include "../VAE/src/core/vae_config.hpp"
#include "../VAE/external/tklb/src/types/THeapBuffer.hpp"
#include "../VAE/external/tklb/src/types/TSpinLock.hpp"


namespace godot { namespace vaeg {

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

	class Emitter : public Spatial {
		GODOT_CLASS(Emitter, Spatial)
	private:
		size_t mTime = 0;
		Ref<AudioStreamSample> mStream;
		tklb::AudioBufferFloat mBuffer;
	public:

		~Emitter() {
			mixerShared.remove(this);
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
			mixerShared.add(this);
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
				auto arrayBuf = s->get_data();
				auto read = arrayBuf.read();
				const int16_t* data = reinterpret_cast<const int16_t*>(read.ptr());
				const int length = arrayBuf.size() / 4;
				// 82,714
				mBuffer.resize(length, 2);
				mBuffer.setFromInterleaved(data, length, 2);
				mBuffer.multiply(0.0001);
				// mBuffer.set(data, length, 0);
				// mBuffer.set(data, length, 1);
			}
		}

		size_t get_time() const {
			return mTime;
		}

		void set_time(const size_t&& time) {
			mTime = time;
		}

		const tklb::AudioBufferFloat& get_buffer() const {
			return mBuffer;
		}

		Ref<AudioStreamSample> get_stream() { return mStream; }

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
					if (!i->mPlaying) { continue; }
					const size_t startTime = i->get_time();
					const auto& buffer = i->get_buffer();
					const size_t totalLength = buffer.size();
					const size_t length = std::min(samples, totalLength - startTime);
					for (int c = 0; c < toDevice.channels(); c++) {
						int channel = c % buffer.channels();
						for (size_t s = 0; s < length; s++) {
							toDevice[c][s] += buffer[c][startTime + s];
						}
					}
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
				mixerShared.unlock();

				time += samples;
			};
			device = new Device(callback); // sync output device
			device->openDevice(2, 2);
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
} } // godot::vaeg



#endif // VAEG_EMITTER

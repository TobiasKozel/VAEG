#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

// #include "AudioStreamGenerator.hpp"
// #include "PoolArrays.hpp"
// #define VAE_NO_AUDIO_THREAD
// #define VAE_NO_AUDIO_DEVICE
#define VAE_FORCE_LOG
#define VAE_LOG_EVENTS
#define VAE_LOG_VOICES

#include "../VAE/src/core/vae_engine.hpp"
#include "String.hpp"
#include <cmath>
#include <gdnative/gdnative.h>

/**
 * This is from godots core/typedefs.h:
 * Windows badly defines a lot of stuff we'll never use. Undefine it.
 */
#ifdef _WIN32
	#undef min // override standard definition
	#undef max // override standard definition
	#undef ERROR // override (really stupid) wingdi.h standard definition
	#undef DELETE // override (another really stupid) winnt.h standard definition
	#undef MessageBox // override winuser.h standard definition
	#undef MIN // override standard definition
	#undef MAX // override standard definition
	#undef CLAMP // override standard definition
	#undef Error
	#undef OK
	#undef CONNECT_DEFERRED // override from Windows SDK, clashes with Object enum
#endif

#include <OS.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <AudioStream.hpp>
#include <AudioStreamGeneratorPlayback.hpp>
#include <AudioStreamPlayer.hpp>


namespace godot {

	vae::core::Engine* engine;
	vae::core::Engine& vae() {
		return *engine;
	}

	class VAEEngine : public Node {
		GODOT_CLASS(VAEEngine, Node)
		// Ref<AudioStreamGenerator> mGenerator;
		// Ref<AudioStreamGeneratorPlayback> mPlayback;
		// AudioStreamPlayer* mPlayer;
		PoolVector2Array mScratch;
	public:
		static void _register_methods() {
			register_signal<VAEEngine>("vae_started");
			register_signal<VAEEngine>("vae_stopped");
			register_method("_process", &VAEEngine::_process);
			register_method("_ready", &VAEEngine::_ready);
			register_method("start", &VAEEngine::start);
			register_method("stop", &VAEEngine::stop);
			register_method("load_bank", &VAEEngine::loadBank);
			register_method("load_hrtf", &VAEEngine::loadHRTF);
			register_method("set_mixer_effect", &VAEEngine::setMixerEffectParameter);
		}

		VAEEngine() {
			// We use the bank from the submodule
			// TODO This should be relative to the godot executable
			// const char* path = "/home/usr/git/master/VAEG/VAE/dev/";
			const char* path = "C:\\dev\\git\\master\\VAEG\\VAE\\dev\\";
			vae::EngineConfig config  = { path, 44100 };
			// config.processInBufferSwitch = false;
			// config.hrtfVoices = 1;
			// config.voices = 2;
			vae().init(config);
			vae().setMasterVolume(2);

		}

		~VAEEngine() { }

		void _init() {
			// mScratch.resize(1024);
			// auto test = mScratch.size();
			// mPlayer = AudioStreamPlayer::_new();
			// // mGenerator = AudioStreamGenerator::_new();
			// mPlayback = AudioStreamGeneratorPlayback::_new();
			// mGenerator = mPlayback;
			// // mGenerator.instance();
			// add_child(mPlayer);
			// mPlayer->set_stream(mGenerator);
			// mPlayer->play();
		}

		void _ready() { }

		bool start() {
			if (vae().start() == vae::Result::Success)
			{
				emit_signal("vae_started");
				return true;
			}
			return false;
		}

		bool stop() {
			if (vae().stop() == vae::Result::Success) {
				emit_signal("vae_stopped");
				return true;
			}
			return false;
		}

		void setMixerEffectParameter(int bank, int mixer, int index, int param, float value) {
			vae().setMixerEffectParameter(bank, mixer, index, param, value);
		}

		bool loadBank(String path) {
			return vae().loadBank(path.utf8().get_data()) == vae::Result::Success;
		}

		bool loadHRTF(String path) {
			return vae().loadHRTF(path.utf8().get_data()) == vae::Result::Success;
		}

		void _process(float delta) {
			vae().update();
			// int needs = mPlayback->get_frames_available();
			// if (needs) {
			// 	auto write = mScratch.write();
			// 	auto ptr = reinterpret_cast<float*>(write.ptr());
			// 	vae().process(needs, ptr, 2);
			// 	mPlayback->push_buffer(mScratch);
			// }
		}
	};
}





#endif // _VAEG_ENGINE

#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

// #define VAE_NO_AUDIO_THREAD
// #define VAE_NO_AUDIO_DEVICE
// #define VAE_NO_LOG
// #define VAE_CUSTOM_PRINT	// log to own function
#define VAE_FORCE_LOG		// also log in release
#define VAE_LOG_EVENTS		// log events
#define VAE_LOG_VOICES		// log voices
#define VAE_IMPL

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
#include <ProjectSettings.hpp>
#include <AudioStream.hpp>
#include <AudioStreamGeneratorPlayback.hpp>
#include <AudioStreamPlayer.hpp>

// void vae_print(vae::LogLevel level, const char* message) {
// 	godot::String m(message);
// 	godot::Godot::print(message);
// }


namespace godot {

	vae::core::Engine* engine;
	vae::core::Engine& vae() {
		return *engine;
	}

	class VAEEngine : public Node {
		GODOT_CLASS(VAEEngine, Node)

		// keep the bank path around since it's used in the engine config struct
		std::string mBankPath;
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
			register_method("set_volume", &VAEEngine::setVolume);
			register_method("set_mixer_effect", &VAEEngine::setMixerEffectParameter);
		}

		VAEEngine() {
			const bool useProjectFolder = !OS::get_singleton()->has_feature("standalone");
			if (useProjectFolder) {
				auto path = ProjectSettings::get_singleton()->globalize_path("res://");
				mBankPath = path.utf8().get_data();
				mBankPath = mBankPath + "banks/";
			} else {
				auto path = OS::get_singleton()->get_executable_path().get_base_dir();
				mBankPath = path.utf8().get_data();
				mBankPath = mBankPath + "/banks/"; // needs a seperator
			}

			vae::EngineConfig config;
			config.rootPath = mBankPath.c_str();
			config.internalSampleRate = 44100;
			vae().init(config);
		}

		~VAEEngine() { }

		void _init() {
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

		void setVolume(float volume) {
			vae().setMasterVolume(volume);
		}

		void _process(float delta) {
			vae().update();
		}
	};
}





#endif // _VAEG_ENGINE

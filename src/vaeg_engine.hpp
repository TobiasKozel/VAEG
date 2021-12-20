#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

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


namespace godot {

	vae::core::Engine* engine;
	vae::core::Engine& vae() {
		return *engine;
	}

	class VAEEngine : public Node {
		GODOT_CLASS(VAEEngine, Node)
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
		}

		VAEEngine() { }

		~VAEEngine() { }

		void _init() { }

		void _ready() { }

		bool start() {
			if (vae().start() == vae::Result::Success) {
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

		bool loadBank(String path) {
			return vae().loadBank(path.utf8().get_data()) == vae::Result::Success;
		}

		bool loadHRTF(String path) {
			return vae().loadHRTF(path.utf8().get_data()) == vae::Result::Success;
		}

		void _process(float delta) {
			// vae().update();
		}
	};
}





#endif // _VAEG_ENGINE

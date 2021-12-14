#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

#include "../VAE/src/core/vae_engine.hpp"

/**
 * Windows badly defines a lot of stuff we'll never use. Undefine it.
 * This if from godots core/typedefs.h
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

namespace godot {
	vae::core::Engine* engine = nullptr;
	std::string rootPath;
	bool vaeRunning = false;

	vae::core::Engine& vae() {
		return *engine;
	}

	void start() {
		if (!vaeRunning) {
			auto exec = OS::get_singleton()->get_executable_path();
			rootPath = exec.utf8().get_data();
			// const char* path = rootPath.c_str();
			const char* path = "C:/dev/git/master/VAEG/demo/banks/";
			engine = new vae::core::Engine({ path });
			vae().loadBank("bank1");
			vae().start();
			vaeRunning = true;
		}
	}

	void stop() {
		if (vaeRunning) {
			delete engine;
			engine = nullptr;
			vaeRunning = false;
		}
	}
}

#endif // _VAEG_ENGINE

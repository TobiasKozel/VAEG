#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

#include "../VAE/src/core/vae_engine.hpp"
#undef CONNECT_DEFERRED // lovely windows.h easter egg, used as an enum in Object.hpp

namespace godot {
	static vae::core::Engine engine({
		"C:\\dev\\git\\master\\VAEG\\VAE\\dev/"
	});
	bool vaeRunning = false;

	vae::core::Engine& vae() {
		return engine;
	}

	void start() {
		if (!vaeRunning) {
			vae().loadBank("bank1");
			vae().start();
			vaeRunning = true;
		}
	}
}

#endif // _VAEG_ENGINE

#ifndef _VAEG_ENGINE
#define _VAEG_ENGINE

#include "../VAE/src/core/vae_engine.hpp"
#undef CONNECT_DEFERRED // lovely windows.h easter egg, used as an enum in Object.hpp

namespace vaeg {
	vae::EngineConfig config {
		"C:\\dev\\git\\master\\VAEG\\VAE\\dev/"
	};
	static vae::core::Engine engine(config);
	bool running = false;

	vae::core::Engine& e() {
		return engine;
	}

	void start() {
		if (!running) {
			e().loadBank("bank1");
			e().start();
			running = true;
		}
	}
}

#endif // _VAEG_ENGINE

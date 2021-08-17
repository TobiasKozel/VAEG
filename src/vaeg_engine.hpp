#ifndef VAEG_ENGINE
#define VAEG_ENGINE

#include "./vaeg_gdnative.hpp"
#include "../VAE/src/core/vae_engine.hpp"

namespace vaeg {
	struct Engine {
		static vae::core::Engine& instance() {
			static vae::core::Engine e;
			return e;
		}
	};
}

#endif // VAEG_ENGINE

#ifndef VAEG_ENGINE
#define VAEG_ENGINE

#include "./vaeg_gdnative.hpp"

namespace vaeg {
	struct Engine {

		static Engine& instance() {
			static Engine e;
			return e;
		}
	};
}

#endif // VAEG_ENGINE

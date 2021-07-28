#include "vaeg_gdnative.hpp"
#include "./vaeg_emitter.hpp"

void vaeg::init_gdnative(void* handle) {
	VAEG_REGISTER_CLASS(Emitter, Spatial)
}

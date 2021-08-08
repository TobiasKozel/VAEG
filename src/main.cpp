#include "vaeg_gdnative.hpp"
#include "./vaeg_emitter.hpp"
#include "./vaeg_listener.hpp"

void vaeg::init_gdnative(void* handle) {
	Emitter::_register(handle);
	Listener::_register(handle);
}

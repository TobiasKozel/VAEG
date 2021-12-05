#include "./vaeg_engine.hpp"
#include "./vaeg_emitter.hpp"
#include "./vaeg_listener.hpp"


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
	if (!o->in_editor) {
		godot::start();
	}
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);
	godot::register_class<godot::VAEEmitter>();
	godot::register_class<godot::VAEListener>();
}

#define VAE_LOG_EVENTS
#define VAE_LOG_VOICES
#include "./vaeg_engine.hpp"
#include "./vaeg_emitter.hpp"
#include "./vaeg_listener.hpp"


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	const char* path = "/home/usr/git/master/VAEG/demo/banks/";
	godot::engine = new vae::core::Engine({ path, 44100 });
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
	delete godot::engine;
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);
	godot::register_class<godot::VAEEmitter>();
	godot::register_class<godot::VAEListener>();
	godot::register_class<godot::VAEEngine>();
}

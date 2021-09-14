#ifndef VAEG_EMITTER
#define VAEG_EMITTER

#include "./vaeg_gdnative.hpp"
#include "../VAE/src/core/vae_emitter.hpp"
#include "./vaeg_engine.hpp"

namespace vaeg {
	godot_method_bind* gdn_bind_get_buffer_data;
	godot_method_bind* gdn_bind_get_rid;
	godot_method_bind* gdn_bind_get_id;
	godot_method_bind* gdn_bind_get_translation;

	struct Emitter {
		tklb::Handle handle;
		vae::core::Emitter* emitter = nullptr;
		using State = vae::core::Emitter::State;
		godot_variant sample;

		VAEG_SETGET_LOC(playing,  bool, emitter->state[State::playing])
		VAEG_SETGET_LOC(loop,     bool, emitter->state[State::loop])
		VAEG_SETGET_LOC(killable, bool, emitter->state[State::killable])
		VAEG_SETGET_LOC(binaural, bool, emitter->state[State::binaural])
		VAEG_SETGET_LOC(canvirt,  bool, emitter->state[State::canvirt])

		Emitter() {
			handle = Engine::instance().createEmitter();
			emitter = Engine::instance().getEmitter(handle);
			api->godot_variant_new_nil(&sample);
		}

		VAEG_REGISTER_CLASS(Emitter, Spatial) {
			VAEG_REGISTER_PROP(playing,  bool, false)
			VAEG_REGISTER_PROP(loop,     bool, false)
			VAEG_REGISTER_PROP(killable, bool, false)
			VAEG_REGISTER_PROP(binaural, bool, false)
			VAEG_REGISTER_PROP(canvirt,  bool, false)
			VAEG_REGISTER_PROP_HINT(
				sample_ref, object, nullptr,
				GODOT_PROPERTY_HINT_RESOURCE_TYPE,
				"AudioStreamSample"
			)
			VAEG_REGISTER_METHOD(_process)
			VAEG_REGISTER_METHOD(_init)

			gdn_bind_get_buffer_data = api->godot_method_bind_get_method("AudioStreamSample", "get_data");
			gdn_bind_get_translation = api->godot_method_bind_get_method("Spatial", "get_translation");
		}

		void updateTransform(double delta = 0) {
			auto old = emitter->position;
			const void* no_args[1] = { };
			api->godot_method_bind_ptrcall(
				gdn_bind_get_translation, instance, no_args, &emitter->position
			);
			emitter->velocity = emitter->position - old;
		}

		VAEG_FUNC(_init, int num_args, godot_variant** args) {
			updateTransform();
			godot_variant ret;
			api->godot_variant_new_nil(&ret);
			return ret;
		}

		VAEG_FUNC(_process, int num_args, godot_variant** args) {
			double delta = api->godot_variant_as_real(args[0]);
			updateTransform(delta);
			godot_variant ret;
			api->godot_variant_new_nil(&ret);
			return ret;
		}

		VAEG_SET(sample_ref, object, godot_object* parameter) {
			api->godot_variant_destroy(&sample);
			api->godot_variant_new_object(&sample, parameter);

			godot_pool_byte_array pool;
			api->godot_pool_byte_array_new(&pool);
			const void* no_args[1] = { };
			api->godot_method_bind_ptrcall(gdn_bind_get_buffer_data, parameter, no_args, &pool);
			godot_int size = api->godot_pool_byte_array_size(&pool);
			const int channels = 2;
			const int length = size / (channels * 2);
			godot_pool_byte_array_read_access* read_access = api->godot_pool_byte_array_read(&pool);
			const uint8_t* data = api->godot_pool_byte_array_read_access_ptr(read_access);

			tklb::Handle h = Engine::instance().createClip();
			auto clip = Engine::instance().getClip(h);
			clip->data.resize(length, 2);
			clip->data.setFromInterleaved(reinterpret_cast<const short*>(data), length, channels);
			clip->data.multiply(1.0 / 32768.0);
			emitter->clip = h;

			api->godot_pool_byte_array_read_access_destroy(read_access);
			api->godot_pool_byte_array_destroy(&pool);
		}

		VAEG_GET(sample_ref) {
			godot_variant ret;
			api->godot_variant_new_nil(&ret);
			return ret;
		}

		~Emitter() {
			Engine::instance().destroyEmitter(handle);
			api->godot_variant_destroy(&sample);
		}
	};
}

#endif // VAEG_EMITTER

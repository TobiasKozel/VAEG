#ifndef VAEG_EMITTER
#define VAEG_EMITTER

#include "./vaeg_gdnative.hpp"
#include "../VAE/src/core/device/vae_rtaudio.hpp"
#include "../VAE/src/core/vae_config.hpp"
#include "../VAE/external/tklb/src/types/THeapBuffer.hpp"
#include "../VAE/external/tklb/src/types/TSpinLock.hpp"

#include <string.h>
#include <stdio.h>

namespace vaeg {
	struct Emitter;



	godot_method_bind* get_buffer_data_bind;
	godot_method_bind* get_rid_bind;
	godot_method_bind* get_id_bind;

	struct Emitter {
		tklb::AudioBufferFloat buffer;
		bool playing = false;
		bool loop = false;
		size_t time = 0;
		int rid = 0;
		godot_variant sample;

		VAEG_SETGET(playing, bool)
		VAEG_SETGET(loop, bool)
		// VAEG_SETGET(Emitter, sample_ref, object)

		Emitter() {
			api->godot_variant_new_nil(&sample);
		}

		VAEG_REGISTER_CLASS(Emitter, Spatial) {
			VAEG_REGISTER_PROP(playing, bool, false)
			VAEG_REGISTER_PROP(loop, bool, false)
			VAEG_REGISTER_PROP_HINT(
				sample_ref, object, nullptr,
				GODOT_PROPERTY_HINT_RESOURCE_TYPE, "AudioStreamSample"
			)
			VAEG_REGISTER_METHOD(do_some)

			get_buffer_data_bind = api->godot_method_bind_get_method("AudioStreamSample", "get_data");
		}

		VAEG_FUNC(do_some) {
			godot_variant ret;
			api->godot_variant_new_nil(&ret);
			return ret;
		}

		VAEG_SET(sample_ref, object) {
			api->godot_variant_destroy(&sample);
			api->godot_variant_new_object(&sample, parameter);

			godot_pool_byte_array pool;
			api->godot_pool_byte_array_new(&pool);
			const void* no_args[1] = { };
			api->godot_method_bind_ptrcall(get_buffer_data_bind, parameter, no_args, &pool);
			godot_int size = api->godot_pool_byte_array_size(&pool);
			const int channels = 2;
			const int length = size / (channels * 2);
			godot_pool_byte_array_read_access* read_access = api->godot_pool_byte_array_read(&pool);
			const uint8_t* data = api->godot_pool_byte_array_read_access_ptr(read_access);

			mixerShared.remove(this);
			buffer.resize(length, 2);
			buffer.setFromInterleaved(reinterpret_cast<const short*>(data), length, channels);
			// buffer.multiply(0.0001f);
			buffer.multiply(1.0 / 32768.0);
			mixerShared.add(this);

			api->godot_pool_byte_array_read_access_destroy(read_access);
			api->godot_pool_byte_array_destroy(&pool);
		}

		VAEG_GET(sample_ref) {
			return sample;
		}

		~Emitter() {
			api->godot_variant_destroy(&sample);
			mixerShared.remove(this);
		}
	};
}

#endif // VAEG_EMITTER

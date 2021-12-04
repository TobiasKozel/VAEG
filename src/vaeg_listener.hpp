#ifndef _VAEG_LISTENER
#define _VAEG_LISTENER

#include "./vaeg_engine.hpp"
#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {
	class VAEListener : public godot::Spatial {
		GODOT_CLASS(VAEListener, godot::Spatial)
		vae::ListenerHandle mListener;
	public:
		static void _register_methods() {
			register_method("_process", &VAEListener::_process);
		}

		void _init() {
		}

		VAEListener() {
			mListener = vae().createListener();
		}

		~VAEListener() {
			vae().removeListener(mListener);
		}

		void _process(float delta) {
			const auto global = get_global_transform();
			const auto pos = global.get_origin();
			vae().setEmitter(mListener, {
				pos.x,
				pos.y,
				pos.z,
			}, { });
			vae().setListener(mListener, { {
				pos.x,
				pos.y,
				pos.z,
			}, { }, { } });
		}

	};
}

#endif // _VAEG_LISTENER

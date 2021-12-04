#ifndef _VAEG_LISTENER
#define _VAEG_LISTENER

#include "./vaeg_engine.hpp"
#include <Godot.hpp>
#include <Spatial.hpp>

namespace vaeg {
	class VAEListener : public godot::Spatial {
		GODOT_CLASS(VAEListener, godot::Spatial)
		vae::ListenerHandle mListener;
	public:
		static void _register_methods() {
			register_method("_process", &VAEListener::_process);
		}

		VAEListener() {
			mListener = vaeg::e().createListener();
		}

		~VAEListener() {
			vaeg::e().removeListener(mListener);
		}

		void _process(float delta) {
			// auto pos = get_translation();
			// vaeg::e().setEmitter(mEmitter, {
			// 	pos.x,
			// 	pos.y,
			// 	pos.z,
			// }, { });
		}

	};
}

#endif // _VAEG_LISTENER

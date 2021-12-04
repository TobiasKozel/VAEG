#ifndef _VAEG_EMITTER
#define _VAEG_EMITTER

#include "./vaeg_engine.hpp"

#include <Godot.hpp>
#include <Spatial.hpp>

namespace vaeg {

	class VAEEmitter : public godot::Spatial {
		GODOT_CLASS(VAEEmitter, godot::Spatial)
		vae::EmitterHandle mEmitter;
		bool mAutoPlay = true;
	public:
		static void _register_methods() {
			register_method("_process", &VAEEmitter::_process);
			// register_property<Emitter, bool>("autoplay", &Emitter::mAutoPlay, false);
		}

		VAEEmitter() {
			mEmitter = vaeg::e().createEmitter();
		}

		~VAEEmitter() {
			vaeg::e().removeEmitter(mEmitter);
		}

		void _init() {
			if (mAutoPlay) {
				play();
			}
		}

		void _process(float delta) {
			auto pos = get_translation();
			vaeg::e().setEmitter(mEmitter, {
				pos.x,
				pos.y,
				pos.z,
			}, { });
		}

		void play() {
			vaeg::e().fireEvent(0, 0, mEmitter);
		}

		void stop() {
			vaeg::e().stopEmitter(mEmitter);
		}

	};

}

#endif // _VAEG_EMITTER

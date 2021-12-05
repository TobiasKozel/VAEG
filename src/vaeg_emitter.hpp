#ifndef _VAEG_EMITTER
#define _VAEG_EMITTER

#include "./vaeg_engine.hpp"

#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {

	class VAEEmitter : public godot::Spatial {
		GODOT_CLASS(VAEEmitter, godot::Spatial)
		vae::EmitterHandle mEmitter;
		bool mAutoPlay = true;
	public:
		static void _register_methods() {
			register_method("_process", &VAEEmitter::_process);
			// register_property<Emitter, bool>("autoplay", &Emitter::mAutoPlay, false);
		}

		VAEEmitter() { }

		~VAEEmitter() {
			vae().removeEmitter(mEmitter);
		}

		void _init() {
			mEmitter = vae().createEmitter();
			if (mAutoPlay) {
				play();
			}
		}

		void _process(float delta) {
			const auto global = get_global_transform();
			const auto pos = global.get_origin();
			vae().setEmitter(mEmitter, {
				pos.x,
				pos.y,
				pos.z,
			}, { });
		}

		void play() {
			vae().fireEvent(0, 0, mEmitter);
		}

		void stop() {
			vae().stopEmitter(mEmitter);
		}

	};

}

#endif // _VAEG_EMITTER

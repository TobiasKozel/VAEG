#ifndef _VAEG_EMITTER
#define _VAEG_EMITTER

#include "./vaeg_engine.hpp"

#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {

	class VAEEmitter : public godot::Spatial {
		GODOT_CLASS(VAEEmitter, godot::Spatial)
		vae::EmitterHandle mEmitter;
		int mBank = vae::InvalidBankHandle;
		int mEvent = vae::InvalidEventHandle;
		float mMaxDistance = 50;
		float mSpread = 0.5;
		bool mAutoEmitter = false;

		void getTransform(vae::LocationDirection& t) {
			const auto global = get_global_transform();
			const auto pos = global.get_origin();
			t = {
				pos.x,
				pos.y,
				pos.z,
			};
		}

	public:
		static void _register_methods() {
			register_method("_process", &VAEEmitter::_process);
			register_method("_ready", &VAEEmitter::_ready);
			register_property<VAEEmitter, bool>("auto_emitter", &VAEEmitter::mAutoEmitter, false);
			register_property<VAEEmitter, int>("bank", &VAEEmitter::mBank, vae::InvalidBankHandle);
			register_property<VAEEmitter, int>("event", &VAEEmitter::mEvent, vae::InvalidEventHandle);
			register_property<VAEEmitter, float>("max_distance", &VAEEmitter::mMaxDistance, 50);
			register_property<VAEEmitter, float>("spread", &VAEEmitter::mSpread, 0.5);
			register_method("play", &VAEEmitter::play);
			register_method("stop", &VAEEmitter::stop);
		}

		VAEEmitter() { }

		~VAEEmitter() {
			vae().removeEmitter(mEmitter);
		}

		void _init() {
			// needed for godot cpp
			// https://github.com/godotengine/godot-cpp/issues/348
			// but too early for class members to be initialized
		}

		void _ready() {
			if (mAutoEmitter) {
				vae::LocationDirection t;
				getTransform(t);
				mEmitter = vae().createAutoEmitter(mBank, mEvent, mMaxDistance, t, mSpread);
			} else {
				mEmitter = vae().createEmitter();
			}
		}

		void _process(float delta) {
			vae::LocationDirection t;
			getTransform(t);
			vae().setEmitter(mEmitter, t, mSpread);
		}

		void play() {
			vae().fireEvent(mBank, mEvent, mEmitter);
		}

		void stop() {
			vae().stopEmitter(mEmitter);
		}

	};

}

#endif // _VAEG_EMITTER

#ifndef _VAEG_LISTENER
#define _VAEG_LISTENER

#include "./vaeg_engine.hpp"
#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {
	class VAEListener : public godot::Spatial {
		GODOT_CLASS(VAEListener, godot::Spatial)
		vae::ListenerHandle mListener;
		int mConfig = (int) vae::SpeakerConfiguration::Headphones;
	public:
		static void _register_methods() {
			register_method("_process", &VAEListener::_process);
			register_property<VAEListener, int>(
				"speaker_configuration",
				&VAEListener::setConfig, &VAEListener::getConfig,
				(int) vae::SpeakerConfiguration::Headphones,
				GODOT_METHOD_RPC_MODE_DISABLED,
				GODOT_PROPERTY_USAGE_DEFAULT,
				GODOT_PROPERTY_HINT_ENUM,
				"Mono, Headphones, Stereo, HRTF, Quadrophonic, Suround"
			);
		}

		void _init() {
			mListener = vae().createListener();
		}

		VAEListener() { }

		~VAEListener() {
			vae().removeListener(mListener);
		}

		void setConfig(int config) {
			mConfig = config;
			vae().setListenerConfiguration(mListener, (vae::SpeakerConfiguration) config);
		}

		int getConfig() {
			return (int) mConfig;
		}

		void _process(float delta) {
			const auto global = get_global_transform();
			const auto pos = global.get_origin();
			const auto& front = global.basis.z;
			const auto& up = global.basis.y;
			const auto& test = front;
			// printf("%f\t%f\t%f\n", test[0], test[1], test[2]);
			vae().setListener(mListener, {
				{
					pos.x, pos.y, pos.z,
				},
				{
					// for some reason these are all negative, flipping the just works
					-front[0], -front[1], front[2]
				},
				{
					-up[0], -up[1], up[2]
				}
			});
		}

	};
}

#endif // _VAEG_LISTENER

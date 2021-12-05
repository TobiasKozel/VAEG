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
			mListener = vae().createListener();
		}

		VAEListener() { }

		~VAEListener() {
			vae().removeListener(mListener);
		}

		void _process(float delta) {
			const auto global = get_global_transform();
			const auto pos = global.get_origin();
			const auto& front = global.basis.z;
			const auto& up = global.basis.y;
			vae().setListener(mListener, {
				{
					pos.x, pos.y, pos.z,
				},
				{
					front[0], front[1], front[2]
				},
				{
					up[0], up[1], up[2]
				}
			});
		}

	};
}

#endif // _VAEG_LISTENER

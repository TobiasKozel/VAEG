#ifndef VAEG_ENGINE
#define VAEG_ENGINE
#include <Godot.hpp>
#include <Node.hpp>

namespace godot { namespace vaeg {
	class EngineSingleton {
		EngineSingleton() {

		}

		~EngineSingleton() {

		}
	public:
		static EngineSingleton& instance() {
			static EngineSingleton engine;
			return engine;
		}
	};


	// class Engine : public Node {
	// 	GODOT_CLASS(Engine, Node)

	// public:

	// 	static void _register_methods() {
	// 	}

	// 	Engine() { }
	// 	~Engine() { }

	// 	void _init() {

	// 	}
	// };
} } // godot::vaeg

#endif // VAEG_ENGINE

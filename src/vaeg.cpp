#include "vaeg.h"

using namespace godot;

void Vaeg::_register_methods() {
	register_method("_process", &Vaeg::_process);
}

Vaeg::Vaeg() {
}

Vaeg::~Vaeg() {
	// add your cleanup here
}

void Vaeg::_init() {
	// initialize any variables here
	time_passed = 0.0;
}

void Vaeg::_process(float delta) {
	time_passed += delta;

	Vector2 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 6.0)), 10.0 + (10.0 * cos(time_passed * 20)));

	set_position(new_position);
}
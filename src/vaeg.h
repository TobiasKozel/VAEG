#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <AudioStreamSample.hpp>

namespace godot {

class Vaeg : public Spatial {
	GODOT_CLASS(Vaeg, Spatial)

private:
	float time_passed;
	Ref<AudioStreamSample> stream;

public:
	static void _register_methods() {
		register_method("_process", &Vaeg::_process);
		// register_property<GDExample, float>("amplitude", &GDExample::amplitude, 10.0);
		register_property<Vaeg, Ref<AudioStreamSample>>(
			"stream",
			&Vaeg::set_stream,
			&Vaeg::get_stream,
			Ref<AudioStreamSample>()
		);
		// register_property<Vaeg, AudioStreamSample>("stream", &Vaeg::set_stream, &Vaeg::get_stream, s);
	}

	Vaeg() { }
	~Vaeg() { }

	void _init() {
		time_passed = 0.0;
	}

	void _process(float delta) {
		time_passed += delta;
		AudioStreamSample& test = *stream.ptr();
		PoolByteArray data = test.get_data();
		int size = data.size();
		Godot::print("This is test " + String(size));

		Vector3 new_position = Vector3(10.0f + (10.0f * sin(time_passed * 6.0)), 10.0f + (10.0 * cos(time_passed * 20)), 1.0f);
		set_translation(new_position);
	}

	void set_stream(Ref<AudioStreamSample> s) {
		stream = s;
	}

	Ref<AudioStreamSample> get_stream() {
		return stream;
	}
};

}

#endif
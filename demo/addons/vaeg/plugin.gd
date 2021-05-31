tool
extends EditorPlugin

var dock

func _enter_tree():
	dock = preload("res://addons/vaeg/custom_dock.tscn").instance()
	add_control_to_bottom_panel(dock, "Mixer")
	
	add_custom_type(
		"CustomAudioSource",
		"Node2D",
		preload("res://addons/vaeg/custom_audio_source.gd"),
		preload("res://icon.png")
	)


func _exit_tree():
	remove_control_from_bottom_panel(dock)
	dock.free()
	remove_custom_type("CustomAudioSource")

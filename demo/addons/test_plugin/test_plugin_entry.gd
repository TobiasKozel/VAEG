tool
extends EditorPlugin

var dock

func _enter_tree():
	dock = preload("res://addons/test_plugin/custom_dock.tscn").instance()
	add_control_to_bottom_panel(dock, "Mixer")
	# add_control_to_dock(DOCK_SLOT_LEFT_UL, dock)
	
	add_custom_type(
		"CustomAudioSource",
		"Node2D",
		preload("res://addons/test_plugin/custom_audio_source.gd"),
		preload("res://icon.png")
	)
	
	


func _exit_tree():
	# remove_control_from_docks(dock)
	remove_control_from_bottom_panel(dock)
	dock.free()
	remove_custom_type("CustomAudioSource")

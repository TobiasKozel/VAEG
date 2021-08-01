tool
extends EditorPlugin

var dock
var tree

func _enter_tree():
	dock = preload("res://addons/vaeg/custom_dock.tscn").instance()
	add_control_to_bottom_panel(dock, "Mixer")
	tree = preload("res://addons/vaeg/Tree.tscn").instance()
	var editor: EditorInterface = get_editor_interface()
	var basecontrol: Panel = editor.get_base_control()
	tree.base_node = basecontrol.get_parent().get_parent()
	tree.editor = editor
	add_control_to_bottom_panel(tree, "Tree")
	editor.inspect_object(tree)
	
	add_custom_type(
		"CustomAudioSource",
		"Node2D",
		preload("res://addons/vaeg/custom_audio_source.gd"),
		preload("res://icon.png")
	)

func _exit_tree():
	remove_control_from_bottom_panel(dock)
	dock.free()
	remove_control_from_bottom_panel(tree)
	tree.free()
	remove_custom_type("CustomAudioSource")

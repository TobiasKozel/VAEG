tool
extends Panel

const plugin_name = "vaeg"

func _input(event):
	if event is InputEventKey and event.pressed:
		var key: InputEventKey = event
		var editor_interface: EditorInterface = get_meta("editor_interface")
		if key.scancode == KEY_F1:
			editor_interface.get_tree().quit()

		if key.shift and key.control:
			if key.scancode == KEY_R:
				var plugin_enabled = editor_interface.is_plugin_enabled(plugin_name)
				if plugin_enabled:
					print("Reloading " + plugin_name)
					editor_interface.set_plugin_enabled(plugin_name, false)
				editor_interface.set_plugin_enabled(plugin_name, true)

			if key.scancode == KEY_I:
				print("test")
				OS.set_window_position(Vector2(-2000, 100))
			#	var split = editor_interface.get_file_system_dock().get_parent_control().get_parent_control()
			#	var tree_control = split.get_children()[0].get_children()[0]
			#	tree_control.set_edited_scene(editor_interface.get_base_control())
			#	editor_interface.inspect_object(tree_control)
			#	editor_interface.inspect_object(editor_interface.get_base_control())

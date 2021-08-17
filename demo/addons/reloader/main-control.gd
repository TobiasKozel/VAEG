tool
extends Panel

const plugin_name = "vaeg"

var listening = false


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
				var base = editor_interface.get_base_control()
				var parent = base.get_parent()
				parent = parent.get_parent()
				if !listening:
					listening = true
					print("Started remote debugger")
					parent.emit_signal("play_pressed")
				else:
					parent.emit_signal("stop_pressed")
					listening = false

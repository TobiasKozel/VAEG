tool
extends Panel

var editor_interface: EditorInterface

func set_editor_interface(interface):
	editor_interface = interface

func _input(event):
	if event is InputEventKey:
		var key: InputEventKey = event
		if key.shift and key.control and key.pressed and key.scancode == KEY_R:
			var plugin_name = "vaeg"
			var plugin_enabled = editor_interface.is_plugin_enabled(plugin_name)
			if plugin_enabled:
				print("Reloading " + plugin_name)
				editor_interface.set_plugin_enabled(plugin_name, false)

			editor_interface.set_plugin_enabled(plugin_name, true)


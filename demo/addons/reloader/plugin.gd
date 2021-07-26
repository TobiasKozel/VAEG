tool
extends EditorPlugin

# Attach the main-control script to the editor window
func _enter_tree():
	var editor: EditorInterface = get_editor_interface()
	var basecontrol: Panel = editor.get_base_control()
	basecontrol.set_meta("editor_interface", editor)
	basecontrol.set_script(preload("res://addons/reloader/main-control.gd"))
	basecontrol.set_process_input(true)


func _exit_tree():
	var editor: EditorInterface = get_editor_interface()
	var basecontrol: Panel = editor.get_base_control()
	basecontrol.set_process_input(false)
	basecontrol.set_script(null)

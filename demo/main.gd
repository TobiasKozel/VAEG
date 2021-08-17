extends Node

func _ready():
	pass # Replace with function body.

func _input(event):
	if event is InputEventKey and event.pressed:
		var key: InputEventKey = event
		if key.scancode == KEY_ESCAPE:
			get_tree().quit()

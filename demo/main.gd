# tool
extends Node

var time: float = 0.0

func _ready():
	pass # Replace with function body.

func _input(event):
	if event is InputEventKey and event.pressed:
		var key: InputEventKey = event
		if key.scancode == KEY_ESCAPE:
			get_tree().quit()


func _process(delta):
	time += delta
	var rad = (sin(time * 0.6) + 1.0) * 6 + 0.1
	$VAEEmitter.translation.x = sin(time * 2) * rad
	$VAEEmitter.translation.z = cos(time * 2) * rad

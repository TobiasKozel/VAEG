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
	$Emitter.translation.x = sin(time * 5) * 4

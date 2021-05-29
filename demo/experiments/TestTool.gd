
extends Sprite

func _process(delta):
	rotation_degrees += 180 * delta

func _ready():
	get_node("../CustomAudioSource").play()

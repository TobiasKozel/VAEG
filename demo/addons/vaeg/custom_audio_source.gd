extends Spatial

var child: AudioStreamPlayer2D = AudioStreamPlayer2D.new()
export var stream: AudioStream setget set_stream
export var sample: AudioStreamSample

func set_stream(s: AudioStream):
	stream = s
	child.stream = s


# Called when the node enters the scene tree for the first time.
func _ready():
	add_child(child)


func play():
	child.play()

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

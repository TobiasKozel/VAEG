extends Spatial

export(NodePath) var emitter_path = ""
export(float) var speed = 0.2

onready var emitter: Spatial = get_node(emitter_path)

func _ready():
	emitter.lowpass = 1.0

func _process(delta):
	var player = get_tree().get_nodes_in_group("Player")
	if (player.empty()):
		return

	player = player[0]
	var playar_pos = player.global_transform.origin
	var self_pos = global_transform.origin
	var space_state = get_world().direct_space_state
	var result = space_state.intersect_ray(self_pos, playar_pos, [player])
	if (result.empty()):
		emitter.lowpass = lerp(emitter.lowpass, 0.0, speed)
	else:
		emitter.lowpass = lerp(emitter.lowpass, 0.95, speed)

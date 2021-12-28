extends Area

export(float) var mix_inside = 1.0
export(float) var delay_inside = 0.05
export(float) var mix_outside = 0.4
export(float) var delay_outside = 1.0
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_ReverbZone_body_entered(body):
	if body.is_in_group("Player"):
		EngineInstance.set_mixer_effect(0, 3, 0, 0, 0.9)
		EngineInstance.set_mixer_effect(0, 3, 0, 1, 0.3)
		EngineInstance.set_mixer_effect(0, 3, 0, 2, mix_inside)
		EngineInstance.set_mixer_effect(0, 3, 0, 3, delay_inside)
		print("To small")


func _on_ReverbZone_body_exited(body):
	if body.is_in_group("Player"):
		print("To normal")
		EngineInstance.set_mixer_effect(0, 3, 0, 0, 0.4) # fb1 
		EngineInstance.set_mixer_effect(0, 3, 0, 1, 0.2) # fb2
		EngineInstance.set_mixer_effect(0, 3, 0, 2, mix_outside) # mix
		EngineInstance.set_mixer_effect(0, 3, 0, 3, delay_outside) # pre

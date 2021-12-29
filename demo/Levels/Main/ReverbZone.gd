extends Area

export(int) var bank = 0
export(int) var mixer = 0
export(int) var effect = 0
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
		EngineInstance.set_mixer_effect(bank, mixer, effect, 0, 1.0)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 1, 0.3)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 2, 1.0)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 3, 0.1)
		print("To small")


func _on_ReverbZone_body_exited(body):
	if body.is_in_group("Player"):
		print("To normal")
		EngineInstance.set_mixer_effect(bank, mixer, effect, 0, 0.3) # fb1 
		EngineInstance.set_mixer_effect(bank, mixer, effect, 1, 0.6) # fb2
		EngineInstance.set_mixer_effect(bank, mixer, effect, 2, 0.4) # mix
		EngineInstance.set_mixer_effect(bank, mixer, effect, 3, 0.4) # pre

extends Area

export(int) var bank = 0
export(int) var mixer = 0
export(int) var effect = 0


func _on_ReverbZone_body_entered(body):
	if body.is_in_group("Player"):
		EngineInstance.set_mixer_effect(bank, mixer, effect, 0, 1.0)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 1, 0.3)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 2, 1.0)
		EngineInstance.set_mixer_effect(bank, mixer, effect, 3, 0.1)



func _on_ReverbZone_body_exited(body):
	if body.is_in_group("Player"):
		EngineInstance.set_mixer_effect(bank, mixer, effect, 0, 0.3) # fb1 
		EngineInstance.set_mixer_effect(bank, mixer, effect, 1, 0.6) # fb2
		EngineInstance.set_mixer_effect(bank, mixer, effect, 2, 0.4) # mix
		EngineInstance.set_mixer_effect(bank, mixer, effect, 3, 0.4) # pre

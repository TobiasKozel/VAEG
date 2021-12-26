extends Spatial

export var fast_close := true

func engine_started():
	pass

func _exit_tree() -> void:
	EngineInstance.stop()

func _ready() -> void:
	EngineInstance.connect("vae_started", self, "engine_started")
	EngineInstance.load_hrtf("hrtf.json")
	EngineInstance.load_bank("bank1")
	EngineInstance.start()

	if !OS.is_debug_build():
		fast_close = false

	if fast_close:
		print("** Fast Close enabled in the 'L_Main.gd' script **")
		print("** 'Esc' to close 'Shift + F1' to release mouse **")

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("ui_cancel") and fast_close:
		get_tree().quit() # Quits the game

	if event.is_action_pressed("mouse_input") and fast_close:
		match Input.get_mouse_mode():
			Input.MOUSE_MODE_CAPTURED:
				Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
			Input.MOUSE_MODE_VISIBLE:
				Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)

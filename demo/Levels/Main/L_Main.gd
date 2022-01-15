extends Spatial

export var fast_close := true

var thread

func load_audio():
	EngineInstance.load_hrtf("hrtf.json")
	EngineInstance.load_bank("bank1")
	EngineInstance.load_bank("bank2")
	EngineInstance.start()

func engine_started():
	pass

func _exit_tree() -> void:
	if (thread):
		thread.wait_to_finish()
	EngineInstance.stop()

func _ready() -> void:
	EngineInstance.connect("vae_started", self, "engine_started")
	EngineInstance.set_volume(2)
	load_audio()
	# Threaded bank loading seems to crash for some reason :/
	#thread = Thread.new()
	#thread.start(self, "load_audio")
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

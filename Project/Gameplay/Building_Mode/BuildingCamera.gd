extends Camera3D

@export var move_speed = 1
var mouse_movement = Vector2(0,0)
var mouse_sensitivity = 0.001

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var look_input = Vector3(-mouse_movement.y, -mouse_movement.x, 0) * mouse_sensitivity
	mouse_movement = Vector2(0,0)
	global_rotation += look_input
	pass

func _physics_process(delta):
	
	var move_input = Vector3(0,0,0)
	move_input.z = (Input.is_action_pressed("move_backward") as float) - (Input.is_action_pressed("move_forward") as float)
	move_input.x = (Input.is_action_pressed("move_right") as float) - (Input.is_action_pressed("move_left") as float)
	
	global_position += global_basis.get_rotation_quaternion() * (move_input * move_speed * delta)

func _input(event):
	if event is InputEventMouseMotion and Input.is_mouse_button_pressed(MOUSE_BUTTON_RIGHT):
		mouse_movement = event.relative
		pass
	pass

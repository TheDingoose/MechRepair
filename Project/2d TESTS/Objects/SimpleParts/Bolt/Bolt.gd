extends PartBase

var offset_angle = 0
var dragging_bolt = false

@export var thread_length = 10
@export var thread_current = 5

func _ready():
	pass # Replace with function body.

func _process(delta):
	if dragging_bolt:
		var do_rotation = offset_angle + Vector2(0,1).angle_to(get_global_mouse_position()- global_position) - global_rotation
		
		#overrotation protections
		if do_rotation > PI:
			do_rotation = do_rotation - 2 * PI
		elif do_rotation < -PI:
			do_rotation = max(do_rotation + 2 * PI, 0)
		
		#call when bolt is fully in
		if do_rotation > 0 and thread_current > 0:
			do_rotation = min((thread_length - thread_current) * 2 * PI, do_rotation)
			offset_angle = global_rotation + do_rotation - Vector2(0,1).angle_to(get_global_mouse_position()- global_position) 
		
		thread_current += do_rotation / PI
		global_rotation += do_rotation
	pass

func _input(event):
	if event is InputEventMouseButton and not event.pressed:
		dragging_bolt = false
		#If it is fully in or out, remove/complete it

func _on_collider_input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton:
		if event.pressed:
			dragging_bolt = true
			offset_angle = global_rotation - Vector2(0,1).angle_to(get_global_mouse_position()- global_position) 

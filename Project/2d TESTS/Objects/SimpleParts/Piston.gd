extends PartBase

var inner
var outer
var inner_resting_pos
var piston_direction = Vector2(1,0)
var grabbing_offset = Vector2(0,0)

var dragging_inner = false
var returning_to_resting = false

#TODO: percentual, according to size instead??
#Or some way to lock into a socket on the other side BUT THAT IS FOR LATER
@export var inner_bounds = Vector2(30, 360)
@export var return_speed = 0.4

func _ready():
	inner = $Inner
	outer = $Outer
	inner_resting_pos = inner.global_position
	piston_direction = (inner.global_position - outer.global_position).normalized()

func _process(delta):
	if dragging_inner:
		var mouse_vector = get_global_mouse_position() - outer.global_position
		var drag_dist = mouse_vector.dot(piston_direction.normalized())
		drag_dist = minf(maxf(drag_dist,inner_bounds.x), inner_bounds.y)
		inner.global_position = outer.global_position + piston_direction * drag_dist
	
func _physics_process(delta):
	if returning_to_resting:
		inner.global_position = lerp(inner.global_position, inner_resting_pos, return_speed)
	if inner.global_position.distance_to(inner_resting_pos) < 0.1:
			inner.global_position = inner_resting_pos
			returning_to_resting = false

func _input(event):
	if event is InputEventMouseButton and not event.pressed:
		dragging_inner = false
		returning_to_resting = true

func _on_inner_collider_input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton:
		if event.pressed:
			dragging_inner = true
			grabbing_offset = inner.global_position - get_global_mouse_position()

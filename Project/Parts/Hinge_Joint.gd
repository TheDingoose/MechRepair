@tool
extends Joint

@export var hinge_axis = Vector3(0,0,1)

# Called when the node enters the scene tree for the first time.
func _ready():
	
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Engine.is_editor_hint():
		#if true:
			#DebugDraw3D.draw_line(global_position - hinge_axis / 2, global_position + hinge_axis / 2, Color(0,1,0))
		pass
	pass

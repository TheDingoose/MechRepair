@tool
extends Part


@export var draw_debug = true

@export var joints = Array([], TYPE_OBJECT, &"Joint", Joint)
var selected = false

#attached to what?
#How do I move?
#Am I fixed?

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func connect_joint(other_part, connection_transform, joint_type = Joint.Attachment.DEBUG):
	
	pass

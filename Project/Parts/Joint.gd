@tool
extends Object
class_name Joint
enum Attachment {DEBUG = 0, WORLD_LOCKED, PISTON, HINGE, FREE, NONE}

@export var part1 = NodePath("")
@export var part1_offset = Vector3(0,0,0)


@export var part2 = NodePath("")
@export var part2_offset = Vector3(0,0,0)

@export var attachment = Attachment.HINGE

@export var movement_plane = Vector3(0,0,1)

#resting position?
#Is this a motorized joint??

#what is the joint type?


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func _get_movement_plane():
	return movement_plane.normalized()

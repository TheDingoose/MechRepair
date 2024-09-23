extends PartDebugDraw

# Called when the node enters the scene tree for the first time.
func _ready():
	#add_cycled_line(Vector3(0,0,0), Vector3(0,0,0), 5)
	#set_draw_debug(false)
	set_draw_debug(true)
	pass # Replace with function body.

func _input(event):
		pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	queue_redraw()
	pass
	

func _draw():
	draw_lines()

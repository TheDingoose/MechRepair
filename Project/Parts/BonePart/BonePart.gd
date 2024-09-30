extends Part

var grab_offset_position = Vector3(0,0,0)
var grabbed = false

func _init():

	pass

func _enter_tree():
	#var h = Hinge.new()
	#h.set_offset(Vector3(0,1,0), get_instance_id())
	#add_hinge(h)
	
	
	if (self == $"../Girder3"):
		attach_part_create_average_hinge($"../Girder2")
	
	if (self == $"../Girder4"):
		attach_part_create_average_hinge($"../Girder3")
	
	GlobalPartDebugDraw.part_to_lines(self)

func _process(delta) :
	
	pass

func _input(event):
	if event is InputEventMouseMotion and Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT) and grabbed:
		var camera = get_viewport().get_camera_3d()
		var a = (global_position + grab_offset_position) - camera.global_position
		var b = camera.basis.z
		var movepos = camera.project_position(get_viewport().get_mouse_position(), abs((b).dot(a)))
		
		var current_trans = global_transform
		current_trans.origin = movepos - grab_offset_position	
		
		if Input.is_key_pressed(KEY_R):
			if (self == $"../Girder2"):
				var hingey = get_hinge_to_part($"../Girder3".get_instance_id())
				hingey.set_transform(hingey.get_transform($"../Girder3".get_instance_id()).rotated(Vector3(0,0,1), 1), $"../Girder3".get_instance_id())
			pass
		
		
		solve_to(current_trans)
		
	
		
		
		#global_position = movepos - grab_offset_position
		
		GlobalPartDebugDraw.clear_lines(true, false)
		GlobalPartDebugDraw.part_to_lines(self)
		
	if event is InputEventMouseButton:
		if !event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
			grabbed = false

func _on_bone_collision_input_event(camera, event, position, normal, shape_idx):
	if event is InputEventMouseButton:
			if event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
				grab_offset_position = position - global_position
				grabbed = true
				pass

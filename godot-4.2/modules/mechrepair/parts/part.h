#ifndef PART
#define PART

#include "scene/3d/node_3d.h"

class Hinge;
class PartDebugDraw;

class Part: public Node3D {
	GDCLASS(Part, Node3D);

private:



protected:

	static void _bind_methods();

public:

	TypedArray<Hinge> hinges;
	float weight = 1.0f;

	void add_hinge(Hinge* h);
	void remove_hinge(Hinge* h);
	bool lock_translation;
	bool lock_rotation;

	Hinge* get_hinge_to_part(ObjectID part_id);

	Array solve_to(Transform3D new_transform, PartDebugDraw *debug);
	bool solve_recursive(Dictionary &part_transforms, Dictionary &part_weight, Dictionary &part_sim_steps, int &simulation_steps, PartDebugDraw *debug);
	bool solve_forward(Dictionary &part_transforms, Dictionary &part_weight, Dictionary &part_sim_steps, int &simulation_steps, PartDebugDraw *debug);


	void attach_part_create_average_hinge(Part *p);

	void set_lock_rotation(bool b);
	bool get_lock_rotation();

	void set_lock_translation(bool b);
	bool get_lock_translation();
};



#endif

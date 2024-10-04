#ifndef PART
#define PART

#include "scene/3d/node_3d.h"

class Hinge;

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

	Array solve_to(Transform3D new_transform);
	bool solve_recursive(Dictionary& part_transforms, Dictionary& part_solved, int& simulation_steps);

	void attach_part_create_average_hinge(Part *p);
};



#endif

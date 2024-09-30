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

	

	bool solve_to(Transform3D new_transform);

	void attach_part_create_average_hinge(Part *p);
};



#endif

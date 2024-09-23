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

	void add_hinge(Hinge* h);
	bool lock_translation;
	bool lock_rotation;

	TypedArray<Hinge> hinges;

	bool solve(Transform3D new_transform);
	
	float weight = 1.0f;
};



#endif

#ifndef HINGE
#define HINGE

#include "core/io/resource.h"
#include "part.h"
//class Part;

class Hinge: public Resource {
	GDCLASS(Hinge, Resource);

private:
protected:
	static void _bind_methods();

public:

	

	Dictionary offsets;

	//ObjectID part_a;
	//ObjectID part_b;
	//Array parts;

	float weight = 1.f;
	//offset
	//hinge_type
	//hinge data??
	//resistance
	//parts
	//rotation limit

	void set_offset(Vector3 offset, ObjectID part_id);
	Vector3 get_offset(ObjectID part_id);
	void detach_part(ObjectID part_id);

};

#endif

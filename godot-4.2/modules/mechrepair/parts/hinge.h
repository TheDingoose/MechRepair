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

	

	Dictionary transforms;

	float weight = 1.f;
	//offset
	//hinge_type
	//hinge data??
	//resistance
	//parts
	//rotation limit

	void set_transform(Transform3D transform, ObjectID part_id);
	Transform3D get_transform(ObjectID part_id);
	Dictionary get_other_transforms(ObjectID part_id);
	void detach_part(ObjectID part_id);

	Array get_other_parts(ObjectID part_id);
};

#endif

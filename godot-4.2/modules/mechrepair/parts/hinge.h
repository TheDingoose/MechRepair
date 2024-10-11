#ifndef HINGE
#define HINGE

#include "core/io/resource.h"
#include "part.h"

#include <stdbool.h>
//class Part;

class Hinge: public Resource {
	GDCLASS(Hinge, Resource);

private:
protected:
	static void _bind_methods();
	
public:

	Dictionary transforms;

	float weight = 1.f;

	//TODO: Make an inherited class for hinge behaviour. Could I have hinge solve itself based on a
	//TODO: transform and a part id?? It would make inheritance calls easier. Though what about advanced
	//TODO: changes that have to be made to the parent??

	//TODO: First hinge to be made is free hinge, meaning everything on rotation

	//TODO: FOR NOW THIS JUST NEEDS TO WORK, JUST USE THE BOOL, GET CLEVER AFTER
	bool locked = false;
	//CHANGE THIS LATER

	//TODO: Its already plenty hard to solve rotation
	//TODO: THE QUESTION IS
	//TODO: SOLVE WITH CCD, SO AM I MOVING THE TIP FROM MYSELF
	//TODO: OR,
	//TODO: MAKE A HYPOTHETICAL SOLUTION THAT GETS APPLIED WHEN ITS CORRECT
	//TODO: (Set the first part to transform, then apply limits, and do that a few times. maybe order based on weight)

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

	bool get_locked();
	void set_locked(bool b);
};

#endif

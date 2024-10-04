#include "hinge.h"

void Hinge::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transform", "part_id"), &Hinge::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "transform", "part_id"), &Hinge::set_transform);
	ClassDB::bind_method(D_METHOD("detach_part", "part_id"), &Hinge::detach_part);

	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_debug"), "set_draw_debug", "get_draw_debug");
}

void Hinge::set_transform(Transform3D transform, ObjectID part_id) {
	transforms[part_id] = transform;
}

Transform3D Hinge::get_transform(ObjectID part_id) {
	return transforms[part_id];
}

Dictionary Hinge::get_other_transforms(ObjectID part_id) {
	Dictionary sets;
	for (int i = 0; i < transforms.size(); i++) {
		if (transforms.keys()[i] != part_id) {
			sets[transforms.keys()[i]] = transforms[transforms.keys()[i]];
		}
	}
	return sets;
}

void Hinge::detach_part(ObjectID part_id) {
	transforms.erase(part_id);
	return;
}

Array Hinge::get_other_parts(ObjectID part_id) {
	Array o = transforms.keys();
	//o.pop_at(o.find(part_id));
	o.erase(part_id);
	return o;
}

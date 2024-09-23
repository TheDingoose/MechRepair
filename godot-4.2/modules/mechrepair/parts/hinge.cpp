#include "hinge.h"

void Hinge::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_offset", "part_id"), &Hinge::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "offset", "part_id"), &Hinge::set_offset);
	ClassDB::bind_method(D_METHOD("detach_part", "part_id"), &Hinge::detach_part);

	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_debug"), "set_draw_debug", "get_draw_debug");
}

void Hinge::set_offset(Vector3 offset, ObjectID part_id) {
	offsets[part_id] = offset;
}

Vector3 Hinge::get_offset(ObjectID part_id) {
	//switch (part_id) {
	//	case part_a:
	//		return offset_a;
	//	case part_b:
	//		return offset_b;
	//	default:
	//	//TODO: print some type of warning
	//		return Vector3(0,0,0);
	//}

	return offsets[part_id];
}

void Hinge::detach_part(ObjectID part_id) {
	offsets.erase(part_id);
	return;
}

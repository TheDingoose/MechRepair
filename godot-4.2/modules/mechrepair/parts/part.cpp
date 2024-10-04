#include "part.h"

#include "hinge.h"


void Part::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_hinge", "hinge"), &Part::add_hinge);
	ClassDB::bind_method(D_METHOD("remove_hinge", "hinge"), &Part::remove_hinge);
	ClassDB::bind_method(D_METHOD("solve_to", "new_transform"), &Part::solve_to);
	ClassDB::bind_method(D_METHOD("attach_part_create_average_hinge", "part"), &Part::attach_part_create_average_hinge);
	ClassDB::bind_method(D_METHOD("get_hinge_to_part", "part_id"), &Part::get_hinge_to_part);

}

void Part::add_hinge(Hinge* h) {
	hinges.push_back(h);
}

void Part::remove_hinge(Hinge *h) {
	h->detach_part(get_instance_id());
	hinges.erase(h);
}

Hinge* Part::get_hinge_to_part(ObjectID part_id) {
	
	for (int i = 0; i < hinges.size(); i++) {
		if (cast_to<Hinge>(hinges[i])->transforms.has(part_id)) {
			return cast_to<Hinge>(hinges[i]);
		}
	}
	return nullptr;
}

Array Part::solve_to(Transform3D new_transform) {
	
	Dictionary part_transforms;
	Dictionary part_solved;
	int simulation_steps = 0;
	float precision = 0.1f;

	part_transforms[get_instance_id()] = new_transform;

	solve_recursive(part_transforms, part_solved, simulation_steps);

	//Apply transforms
	for (int i = 0; i < part_transforms.size(); i++) {
		Part* o = cast_to<Part>(ObjectDB::get_instance(part_transforms.keys()[i]));
		o->set_global_transform(part_transforms[part_transforms.keys()[i]]);
	}
	return part_transforms.keys();
}

bool Part::solve_recursive(Dictionary& part_transforms, Dictionary& part_solved, int& simulation_steps) {

	for (int i = 0; i < hinges.size(); i++) {
		Array connections = cast_to<Hinge>(hinges[i])->get_other_parts(get_instance_id());

		for (int j = 0; j < connections.size(); j++) {

			//just solid hinge for now
			if (!part_transforms.keys().has(connections[j])) {
				Transform3D target = part_transforms[get_instance_id()];
				target = target * cast_to<Hinge>(hinges[i])->get_transform(get_instance_id()).inverse();
				target = target * cast_to<Hinge>(hinges[i])->get_transform(connections[j]);

				part_transforms[connections[j]] = target;

				cast_to<Part>(ObjectDB::get_instance(connections[j]))->solve_recursive(part_transforms, part_solved, simulation_steps);
			}
		}
	}

	return false;
}

void Part::attach_part_create_average_hinge(Part *p) {
	Hinge* h = new Hinge;

	Vector3 center = (get_global_position() + p->get_global_position()) * Vector3(0.5, 0.5, 0.5);
	add_hinge(h);
	p->add_hinge(h);

	Transform3D a;
	a = get_global_transform();
	a.origin = (get_global_position() - center);


	h->set_transform(a, get_instance_id());


	a = p->get_global_transform();
	a.origin =(p->get_global_position() - center);

	h->set_transform(a, p->get_instance_id());

	h = nullptr;
}

#include "part.h"

#include "hinge.h"


void Part::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_hinge", "hinge"), &Part::add_hinge);
	ClassDB::bind_method(D_METHOD("remove_hinge", "hinge"), &Part::remove_hinge);
	ClassDB::bind_method(D_METHOD("solve_to", "new_transform"), &Part::solve_to);
	ClassDB::bind_method(D_METHOD("attach_part_create_average_hinge", "part"), &Part::attach_part_create_average_hinge);
}

void Part::add_hinge(Hinge* h) {
	hinges.push_back(h);
}

void Part::remove_hinge(Hinge *h) {
	h->detach_part(get_instance_id());
	hinges.erase(h);
}

bool Part::solve_to(Transform3D new_transform) {
	
	Dictionary part_transforms;
	Dictionary part_solved;
	int simulation_steps = 0;
	float precision = 0.1f;

	part_transforms[this] = new_transform;

	for (int i = 0; i < hinges.size(); i++) {
		Array connections = cast_to<Hinge>(hinges[i])->get_other_parts(get_instance_id());
		for (int j = 0; j < connections.size(); j++) {
			//just solid hinge for now

			Transform3D t = cast_to<Part>(ObjectDB::get_instance(connections[j]))->get_global_transform();
			//Transform3D a = part_transforms[this];
			
			//Very annoying to operate on transforms gotten from dictionaries
			t.origin = Vector3(0,0,0);
			

			Transform3D base = part_transforms[this];
			Vector3 hinge_pos_global = base.xform(cast_to<Hinge>(hinges[i])->get_offset(get_instance_id()));



			t.origin = hinge_pos_global - t.basis.xform(cast_to<Hinge>(hinges[i])->get_offset(connections[j]));

			//t.origin = hinge_pos_global + t.xform(-cast_to<Hinge>(hinges[i])->get_offset(connections[j])); 


			//t.origin = base.xform(cast_to<Hinge>(hinges[i])->get_offset(get_instance_id()) - cast_to<Hinge>(hinges[i])->get_offset(connections[j]));



			//t.origin = a.get_origin() + cast_to<Hinge>(hinges[i])->get_offset(get_instance_id()) - cast_to<Hinge>(hinges[i])->get_offset(connections[j]);
			//t.origin = a.get_origin() + cast_to<Hinge>(hinges[i])->get_offset(get_instance_id()) - cast_to<Hinge>(hinges[i])->get_offset(connections[j]);

			//
			part_transforms[cast_to<Part>(ObjectDB::get_instance(connections[j]))] = t;
		}
	}


	//get a parts movement capabilities
	//get is as close as possible to goal
	//go down the line to find the rest of the solve, if necessary
	


	//Apply transforms
	for (int i = 0; i < part_transforms.size(); i++) {
		Part* o = cast_to<Part>(part_transforms.keys()[i]);
		o->set_global_transform(part_transforms[o]);
	}

	return false;
}

void Part::attach_part_create_average_hinge(Part *p) {
	Hinge* h = new Hinge;

	print_line(get_global_position());
	print_line(p->get_global_position());

	Vector3 center = (get_global_position() + p->get_global_position()) * Vector3(0.5, 0.5, 0.5);
	add_hinge(h);
	p->add_hinge(h);
	print_line(center);
	h->set_offset(get_global_transform().inverse().xform(center), get_instance_id());
	print_line(h->get_offset(get_instance_id()));

	h->set_offset(p->get_global_transform().inverse().xform(center), p->get_instance_id());
	print_line(h->get_offset(p->get_instance_id()));

	h = nullptr;
}

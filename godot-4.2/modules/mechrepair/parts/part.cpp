#include "part.h"

#include "hinge.h"
#include "../debug/part_debug_draw.h"



constexpr int MAX_SIMULATION_STEPS = 500;

void Part::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_hinge", "hinge"), &Part::add_hinge);
	ClassDB::bind_method(D_METHOD("remove_hinge", "hinge"), &Part::remove_hinge);
	ClassDB::bind_method(D_METHOD("solve_to", "new_transform"), &Part::solve_to);
	ClassDB::bind_method(D_METHOD("attach_part_create_average_hinge", "part"), &Part::attach_part_create_average_hinge);
	ClassDB::bind_method(D_METHOD("get_hinge_to_part", "part_id"), &Part::get_hinge_to_part);
	ClassDB::bind_method(D_METHOD("set_lock_rotation", "b"), &Part::set_lock_rotation);
	ClassDB::bind_method(D_METHOD("get_lock_rotation"), &Part::get_lock_rotation);
	ClassDB::bind_method(D_METHOD("set_lock_translation", "b"), &Part::set_lock_translation);
	ClassDB::bind_method(D_METHOD("get_lock_translation"), &Part::get_lock_translation);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lock_rotation"), "set_lock_rotation", "get_lock_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lock_translation"), "set_lock_translation", "get_lock_translation");

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

Array Part::solve_to(Transform3D new_transform, PartDebugDraw* debug) {
	
	Dictionary part_transforms;
	//Dictionary part_solved;
	Dictionary part_weight;
	Dictionary part_sim_steps;

	int simulation_steps = 0;
	float precision = 0.1f;

	//TODO: Buffer of hinges/parts to restore if it went wrong

	part_transforms[get_instance_id()] = new_transform;
	set_global_transform(new_transform);
	part_weight[get_instance_id()] = weight;

	//We can control the amount of loops here
	solve_recursive(part_transforms, part_weight, part_sim_steps, simulation_steps, debug);

	//Apply transforms
	//for (int i = 0; i < part_transforms.size(); i++) {
	//	Part* o = cast_to<Part>(ObjectDB::get_instance(part_transforms.keys()[i]));
	//	o->set_global_transform(part_transforms[part_transforms.keys()[i]]);
	//}

	return part_transforms.keys();
}

bool Part::solve_recursive(Dictionary &part_transforms, Dictionary &part_weight, Dictionary &part_sim_steps, int &simulation_steps, PartDebugDraw *debug) {
	
	if (simulation_steps > MAX_SIMULATION_STEPS) {
		return false;
	}	
	

	for (int i = 0; i < hinges.size(); i++) {
		Array connections = cast_to<Hinge>(hinges[i])->get_other_parts(get_instance_id());

		for (int j = 0; j < connections.size(); j++) {

			//just solid hinge for now

			if (!part_transforms.keys().has(connections[j])) {

				if (cast_to<Hinge>(hinges[i])->locked) {
					Transform3D target = part_transforms[get_instance_id()];
					target = target * cast_to<Hinge>(hinges[i])->get_transform(get_instance_id()).affine_inverse();
					target = target * cast_to<Hinge>(hinges[i])->get_transform(connections[j]);

					//part_weight[connections[j]] = *cast_to<float>(part_weight[get_instance_id()]) + weight;
					part_transforms[connections[j]] = target;
					cast_to<Part>(ObjectDB::get_instance(connections[j]))->set_global_transform(target);

					simulation_steps++;
					cast_to<Part>(ObjectDB::get_instance(connections[j]))->solve_recursive(part_transforms, part_weight, part_sim_steps, simulation_steps,debug);
				} else {

					//This part space
					Transform3D target = part_transforms[get_instance_id()];

					//To hinge space
					target = target * cast_to<Hinge>(hinges[i])->get_transform(get_instance_id()).affine_inverse();

					//To part space
					Transform3D center = cast_to<Part>(ObjectDB::get_instance(connections[j]))->get_global_transform();

					//To hinge space
					Transform3D hinge = center * cast_to<Hinge>(hinges[i])->get_transform(connections[j]).affine_inverse();

					debug->add_point(center.get_origin(), Color(1,0,0));
					debug->add_point(hinge.get_origin(), Color(.75, .25, 0));
					debug->add_point(target.get_origin(), Color(.5, .5, 0));

					//Get different 
					Vector3 center_to_hinge = hinge.get_origin() - center.get_origin();
					Vector3 center_to_target = target.get_origin() - center.get_origin(); 

					Vector3 diff_vector = center_to_hinge.cross(center_to_target);
					float diff_angle = Math::atan2(diff_vector.length(), center_to_hinge.dot(center_to_target));

					diff_vector = hinge.affine_inverse().get_basis().xform((diff_vector));
					diff_vector = diff_vector.normalized();

					debug->add_line(hinge.get_origin(), hinge.get_origin() + (diff_vector * 0.2), Color(0,1,0));

					cast_to<Hinge>(hinges[i])->set_transform(cast_to<Hinge>(hinges[i])->get_transform(connections[j]).rotated(diff_vector, diff_angle), connections[j]);


					part_transforms[connections[j]] = target * cast_to<Hinge>(hinges[i])->get_transform(connections[j]);


					cast_to<Part>(ObjectDB::get_instance(connections[j]))->set_global_transform(part_transforms[connections[j]]);
					simulation_steps++;
					cast_to<Part>(ObjectDB::get_instance(connections[j]))->solve_recursive(part_transforms, part_weight, part_sim_steps, simulation_steps, debug);
				}
			}
		}
	}

	return true;
}

bool Part::solve_forward(Dictionary &part_transforms, Dictionary &part_weight, Dictionary &part_sim_steps, int &simulation_steps, PartDebugDraw *debug) {





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

void Part::set_lock_rotation(bool b) {
	lock_rotation = b;
}

bool Part::get_lock_rotation() {
	return lock_rotation;
}

void Part::set_lock_translation(bool b) {
	lock_translation = b;
}

bool Part::get_lock_translation() {
	return lock_translation;
}

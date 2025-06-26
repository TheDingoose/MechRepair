#include "part_solver.h"
#include "hinge.h"
#include "part.h"

//#include<modules/gdscript/gdscript_utility_functions.h>

#include "../debug/part_debug_draw.h"


void PartSolver::sim_step(ObjectID part_id) {
	part_sim_steps[part_id] = part_sim_steps[part_id] + 1;
	max_reached_part_sim_steps = MAX(part_sim_steps[part_id], max_reached_part_sim_steps);
	simulation_steps++;
}

bool PartSolver::add_target(ObjectID part_id, Transform3D goal) {
	//TODO: Remove this for when we do multiple targets
	part_targets.clear();
	part_targets[part_id] = goal;
	return true;
}

void PartSolver::part_step_debug(PartDebugDraw *debug) {
	for (int i = 0; i < part_transforms.size(); i++) {
		//set parts transform
		Part *set_part = Part::get_instance(part_transforms.keys()[i]);
		set_part->set_global_transform(part_transforms[part_transforms.keys()[i]]);
		debug->debug_part_to_lines_cycled_max(set_part, simulation_steps, max_simulation_steps);
	}
}

bool PartSolver::register_web() {

	//Run through all the parts and register them and their weights
	//Perhaps break them up into sections
	for (int i = 0; i < part_targets.size(); i++) {
		register_part_recursive(part_targets.keys()[i]);
	}
	return true;
}

bool PartSolver::register_part_recursive(ObjectID part_id) {

	if (!part_transforms.keys().has(part_id)) { // Has this part already been added?
		return false;
	}

	Part *this_part = Part::get_instance(part_id);

	part_transforms[part_id] = this_part->get_global_transform();
	part_weight[part_id] = this_part->weight; // Could replace this with local weight
	part_sim_steps[part_id] = 0;
	if (this_part->lock_translation || this_part->lock_rotation) {
		locked_parts.push_back(part_id);
	}

	for (int j = 0; j < this_part->hinges.size(); j++) {
		//run through all connections and add them to the dicts
		Array found_connections = cast_to<Hinge>(this_part->hinges[j])->get_other_parts(part_id);
		for (int k = 0; k < found_connections.size(); k++) {
			register_part_recursive(found_connections[k]);
		}
	}

	return true;
}

bool PartSolver::register_parts_from_target() {

	part_transforms.clear();
	part_weight.clear();
	part_sim_steps.clear();
	part_target_distance.clear();
	locked_parts.clear();
	part_hinge_offsets.clear();
	max_reached_part_sim_steps = 0;

	//floodfill type beat
	Vector<ObjectID> parts_found;
	Vector<ObjectID> hinges_found;

	Vector<ObjectID> previous_layer;
	Vector<ObjectID> current_layer;
	int layer_number = 0;

	for (int i = 0; i < part_targets.size(); i++) {
		parts_found.push_back(part_targets.keys()[i]);
		current_layer.push_back(part_targets.keys()[i]);
	}

	//Floodfill to get the furthest part(s) from the end
	while (current_layer.size() > 0 && layer_number < 20) {
		layer_number++;
		previous_layer = current_layer;
		current_layer.clear();

		for (int i = 0; i < previous_layer.size(); i++) {

			Part *this_part = Part::get_instance(previous_layer[i]);

			part_transforms[previous_layer[i]] = this_part->get_global_transform();
			part_weight[previous_layer[i]] = this_part->weight; // Could replace this with local weight
			part_sim_steps[previous_layer[i]] = 0;
			if (this_part->lock_translation || this_part->lock_rotation) {
				locked_parts.push_back(previous_layer[i]);
			}
			part_target_distance[previous_layer[i]] = layer_number;


			for (int j = 0; j < this_part->hinges.size(); j++) {

				part_hinge_offsets[previous_layer[i]][cast_to<Hinge>(this_part->hinges[j])->get_instance_id()] = cast_to<Hinge>(this_part->hinges[j])->get_transform(previous_layer[i]);

				//run through all connections and add them to the new layer
				Array found_connections = cast_to<Hinge>(this_part->hinges[j])->get_other_parts(previous_layer[i]);
				for (int k = 0; k < found_connections.size(); k++) {
					if (!parts_found.has(found_connections[k])) {
						parts_found.push_back(found_connections[k]);
						current_layer.push_back(found_connections[k]);
					}
				}
			}
		}
	}
	return true;
}

ObjectID PartSolver::find_starting_part() {
	//floodfill type beat
	Vector<ObjectID> parts_found;

	Vector<ObjectID> previous_layer;
	Vector<ObjectID> current_layer;
	int layer_number = 0;


	for (int i = 0; i < part_targets.size(); i++) {
		parts_found.push_back(part_targets.keys()[i]);
		current_layer.push_back(part_targets.keys()[i]);
	}

	//Floodfill to get the furthest part(s) from the end
	while (current_layer.size() > 0) {
		layer_number++;
		previous_layer = current_layer;
		current_layer.clear();

		for (int i = 0; i < previous_layer.size(); i++) {

			Part *this_part = Part::get_instance(previous_layer[i]);

			for (int j = 0; j < this_part->hinges.size(); j++) {
				//run through all connections and add them to the dicts
				Array found_connections = cast_to<Hinge>(this_part->hinges[j])->get_other_parts(previous_layer[i]);
				for (int k = 0; k < found_connections.size(); k++) {
					if (!parts_found.has(found_connections[k])) {
						parts_found.push_back(found_connections[k]);
						current_layer.push_back(found_connections[k]);

					}
				}
			}
		}
	}

	return previous_layer[rand() % previous_layer.size()];
}

//branch PartSolver::find_shortest_path_to_target(ObjectID self, ObjectID hinge) {
//
//	//just go, whenever theres a split, take the path with the part of least targetDist
//
//
//	return branch();
//}

bool PartSolver::solve_ccd(PartDebugDraw *debug) {


	register_parts_from_target();

	Vector<ObjectID> parts_forward_applied;
	simulation_steps = 0;

	Part *target = Part::get_instance(part_targets.keys()[0]);

	ObjectID last_part = part_targets.keys()[0];
	//TODO: Change to random??
	ObjectID this_part = cast_to<Hinge>(target->hinges[0])->get_other_parts(last_part)[0];

	int target_hinge_connections = target->hinges.size();



	//TODO: splits may be a problem down the line, and a reason to do recursion.

	bool solved = false;

	//Forwards solve all. (Or do we attempt a ccd bounce solution??)
	//then, from last part, Backwards solve all
	while (simulation_steps < max_simulation_steps && !solved) {
		print_line("Fo");
		//forward solve
		part_transforms[part_targets.keys()[0]] = part_targets[part_targets.keys()[0]];
		sim_step(part_targets.keys()[0]);
		parts_forward_applied.clear();
		parts_forward_applied.push_back(last_part);

		solved = forward_recursive(true, last_part, &parts_forward_applied, debug);

		//DEBUG ONLY
		part_step_debug(debug);

		//backwards solve
		if (target_hinge_connections > 1) {
			//TODO: THIS IS DEFINITELY NOT RIGHT, CHECK IF THIS IS CORRECT WITH BACKWARDS IN PAPER
			parts_forward_applied.clear();
			print_line("Ba");
			ObjectID that_part = cast_to<Hinge>(target->hinges[1])->get_other_parts(last_part)[0];

			//parts_forward_applied.push_back(this_part);
			//IK_step(this_part, last_part);
			solved = forward_recursive(false, last_part, &parts_forward_applied, debug);
		}

		//DEBUG ONLY
		part_step_debug(debug);
	}
	print_line(simulation_steps);

	for (int i = 0; i < part_transforms.size(); i++) {
		//set parts transform
		Part *set_part = Part::get_instance(part_transforms.keys()[i]);
		set_part->set_global_transform(part_transforms[part_transforms.keys()[i]]);
		debug->part_to_lines_cycled_max(set_part, part_sim_steps[part_transforms.keys()[i]], max_reached_part_sim_steps);

		for (int j = 0; j < set_part->hinges.size(); j++) {
			//set hinge offsets
			cast_to<Hinge>(set_part->hinges[j])->set_transform(part_hinge_offsets[part_transforms.keys()[i]][cast_to<Hinge>(set_part->hinges[j])->get_instance_id()], part_transforms.keys()[i]);
		}

	}

	//debug->part_to_lines_color(cast_to<Part>(ObjectDB::get_instance(find_starting_part())), Color(1, 0, 1));

	return solved;
}

bool PartSolver::forward_recursive(bool forward, ObjectID this_id, Vector<ObjectID>* parts_forward_applied, PartDebugDraw *debug) {

	//TODO: THIS SHOULD HAPPEN INTO THE OTHER DIRECTION CURRENTLY WERE PULLING AND NOT SOLVING THE CONNECTIONS FROM BASE

	bool solved = true;

	//parts_forward_applied->push_back(this_id);

	Array connected_parts = Part::get_instance(this_id)->get_connected_parts();

	if (forward) {
		//Get the next hinges
		for (int i = 0; i < connected_parts.size(); i++) {
			if (!parts_forward_applied->has(connected_parts[i])) {

				solved = IK_step(this_id, connected_parts[i]) && solved;
				parts_forward_applied->push_back(connected_parts[i]);

				solved = forward_recursive(forward, connected_parts[i], parts_forward_applied, debug) && solved;
			}
		}
	}
	else {
		//Get the next hinges
		for (int i = connected_parts.size() - 1; i >= 0; i--) {
			if (!parts_forward_applied->has(connected_parts[i])) {

				solved = IK_step(this_id, connected_parts[i]) && solved;
				parts_forward_applied->push_back(connected_parts[i]);

				solved = forward_recursive(forward, connected_parts[i], parts_forward_applied, debug) && solved;
			}
		}
	}




	//return false;
	return solved;
}

bool PartSolver::IK_step(ObjectID last_id, ObjectID this_id) {

	Hinge *hinge_to_last = Part::get_instance(this_id)->get_hinge_to_part(last_id);

	//calculate from this to hinge space
	Transform3D this_transform = part_transforms[this_id];
	Transform3D this_hinge_transform = this_transform * part_hinge_offsets[this_id][hinge_to_last->get_instance_id()].inverse();

	//calculate from target to hinge space
	Transform3D target_transform = part_transforms[last_id];
	Transform3D target_hinge_transform = target_transform * part_hinge_offsets[last_id][hinge_to_last->get_instance_id()].inverse();

	//calculate the to hinge difference from both directions
	Vector3 this_to_this_hinge = this_hinge_transform.get_origin() - this_transform.get_origin();
	Vector3 this_to_target_hinge = target_hinge_transform.get_origin() - this_transform.get_origin();

	//create rotation of the difference between the two vectors
	Vector3 diff_vector = this_to_this_hinge.cross(this_to_target_hinge);


	float diff_angle = Math::atan2(diff_vector.length(), this_to_this_hinge.dot(this_to_target_hinge));


	if (this_to_this_hinge.distance_to(this_to_target_hinge) > precision && diff_vector != Vector3(0, 0, 0)) {
		//set the diff vector from global, to hinge space

		Vector3 this_diff_vector = this_hinge_transform.inverse().get_basis().xform((diff_vector));
		this_diff_vector = this_diff_vector.normalized();
		//Vector3 this_diff_vector = diff_vector.normalized();

		sim_step(this_id);

		part_hinge_offsets[this_id][hinge_to_last->get_instance_id()] = part_hinge_offsets[this_id][hinge_to_last->get_instance_id()].rotated(this_diff_vector, diff_angle);
		part_transforms[this_id] = target_hinge_transform * part_hinge_offsets[this_id][hinge_to_last->get_instance_id()];


		return false;
	}


	return true;
}

//part_transforms[this_id] = target_hinge_transform * part_hinge_offsets[this_id][hinge_to_last->get_instance_id()];

void PartSolver::_bind_methods() {
	ClassDB::bind_method(D_METHOD("solve_ccd", "debug"), &PartSolver::solve_ccd);
	ClassDB::bind_method(D_METHOD("add_target", "part_id", "transform"), &PartSolver::add_target);
}

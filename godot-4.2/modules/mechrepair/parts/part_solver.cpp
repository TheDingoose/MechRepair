#include "part_solver.h"
#include "hinge.h"
#include "part.h"

void PartSolver::_bind_methods() {

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

	if (!part_sim_steps.keys().has(part_id)) { // Has this part already been added?
		return false;
	}

	Part *this_part = cast_to<Part>(ObjectDB::get_instance(part_id));

	part_transforms[part_id] = this_part->get_global_transform();
	part_weight[part_id] = this_part->weight; // Could replace this with local weight
	part_sim_steps[part_id] = 0;
	if (this_part->lock_translation || this_part->lock_rotation) {
		locked_parts.push_back(part_id); 
	}

	for (int j = 0; j < this_part->hinges.size(); j++) {
		//run through all connections and add them to the dicts
		Array connections = cast_to<Hinge>(this_part->hinges[j])->get_other_parts(part_id);
		for (int k = 0; k < connections.size(); k++) {
			register_part_recursive(connections[k]);
		}
	}

	return true;
}



bool PartSolver::solve_ccd() {
	//Start from "base" <-- should be locked parts, ideally
	//For every step,

	//From root, Check if we can move to close in on end effectors
	//Run down the lines and check if we can move them closer to end effectors

	//The end effectors in this case, are the hinges on the part_targets

	//Check validity, and either move the part_targets, backwards. Or change the effectors for a new solve
	//


	return false;
}

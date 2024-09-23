#include "part.h"

#include "hinge.h"


void Part::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_hinge", "hinge"), &Part::add_hinge);
}

void Part::add_hinge(Hinge* h) {
	hinges.push_back(h);
}

bool Part::solve(Transform3D new_transform) {
	
	Dictionary part_transforms;
	Dictionary part_solved;
	int simulation_steps = 0;
	float precision = 0.1f;

	//get a parts movement capabilities
	//get is as close as possible to goal
	//go down the line to find the rest of the solve, if necessary

	return false;
}

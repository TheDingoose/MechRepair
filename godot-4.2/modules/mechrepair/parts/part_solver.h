#ifndef PARTSOLVER
#define PARTSOLVER

#include "scene/3d/node_3d.h"
#include "scene/main/node.h"

class PartDebugDraw;

//struct branch {
//	ObjectID start_hinge;
//	ObjectID end_hinge;
//
//	TypedArray<ObjectID> parts;
//};

class PartSolver : public Node {
	GDCLASS(PartSolver, Node);


	private:


	protected:

	static void _bind_methods();

	public:
	Dictionary part_targets;
	Dictionary part_transforms;
	HashMap<ObjectID, float> part_weight;
	HashMap<ObjectID, unsigned int> part_sim_steps;
	HashMap<ObjectID, unsigned int> part_target_distance;
	Array locked_parts;

	HashMap<ObjectID, HashMap<ObjectID, Transform3D>> part_hinge_offsets;
	//hinge limits as well

	unsigned int max_reached_part_sim_steps = 0;
	int simulation_steps = 0;
	const int max_simulation_steps = 50;
	const float precision = 0.001f;

	void sim_step(ObjectID part_id);
	bool add_target(ObjectID part_id, Transform3D goal);
	void part_step_debug(PartDebugDraw *debug);
	bool register_web();
	bool register_part_recursive(ObjectID part_id); // Note, does not set part_targetDistance
	bool register_parts_from_target();
	ObjectID find_starting_part();
	//branch find_shortest_path_to_target(ObjectID self, ObjectID hinge);
	bool solve_ccd(PartDebugDraw *debug);
	bool forward_recursive(bool forward, ObjectID this_id, Vector<ObjectID>* parts_forward_applied, PartDebugDraw *debug);
	bool IK_step(ObjectID last_id, ObjectID this_id);
	//Handlers for hinges? What can the movement of the part be towards the end result??



	};

#endif

#ifndef PARTSOLVER
#define PARTSOLVER

#include "scene/3d/node_3d.h"
#include "scene/main/node.h"



class PartSolver : public Node {
	GDCLASS(PartSolver, Node);


	private:


	protected:

	static void _bind_methods();

	public:
	Dictionary part_targets;
	Dictionary part_transforms;
	Dictionary part_weight;
	Dictionary part_sim_steps;
	Array locked_parts; 
	//Vector<Vecotr<PartIDs>> Branches
	//Branch prio?


	int simulation_steps = 0;
	float precision = 0.1f;


	bool register_web();
	bool register_part_recursive(ObjectID part_id);
	//bool build_branches();
	bool solve_ccd();
	
	};

#endif

#include "register_types.h"

#include "core/object/class_db.h"


#include "parts/part.h"
#include "parts/hinge.h"
#include "debug/part_debug_draw.h"
#include "parts/part_solver.h"


void initialize_mechrepair_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<DebugLine>();
	ClassDB::register_class<CycledLine>();
	ClassDB::register_class<Part>();
	ClassDB::register_class<Hinge>();
	ClassDB::register_class<PartDebugDraw>();
	ClassDB::register_class<PartSolver>();
}

void uninitialize_mechrepair_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Nothing to do here in this example.
}

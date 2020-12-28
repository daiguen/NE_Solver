#include "NE_Domain.h"
#include "NE_Solver.h"
#include "NE_Component.h"

void NE_Domain::init(void) {
	set_id(0);
	set_param_id(0);
	_throw_id.clear();
	_throw_list.clear();
}

void NE_Domain::set(NE_Solver& solver) {
	// throw
	vector<int>::iterator th_it;
	vector<int>& th_ = get_throw_id();

	vector<NE_Throw*>::iterator throw_it;
	vector<NE_Throw*>& throw_ = solver.get_throw_list();
	for (th_it = th_.begin(); th_it != th_.end(); th_it++) {
		for (throw_it = throw_.begin(); throw_it != throw_.end(); throw_it++) {
			if (*(th_it) == (**throw_it).get_id()) {
				(*throw_it)->set(solver);
				add_throw(*throw_it);				
				break;
			}
		}		
	}
}

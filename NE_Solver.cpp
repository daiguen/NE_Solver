#include "NE_Solver.h"
#include "NE_Input_Reader.h"
#include "NE_Domain.h"
#include "NE_Component.h"

void NE_Solver::set(string inp) {
	_input_reader = new NE_Input_Reader;
	_dom = new NE_Domain;

	NE_Input_Reader& inp_reader = get_input_reader();
	NE_Domain& dom = get_domain();

	inp_reader.set(inp);
	inp_reader.read(*this);
	dom.set(*this);
}

void NE_Solver::run(void) {
	NE_Domain& dom = get_domain();

	vector<NE_Throw*>::iterator throw_it;
	vector<NE_Throw*>& throw_ = dom.get_throw_list();

	for (throw_it = throw_.begin(); throw_it != throw_.end(); throw_it++) {
		(**throw_it).calculate(dom.get_parameters());
	}
}
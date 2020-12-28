#include "NE_Component.h"
#include "NE_Error_Handler.h"
#include "NE_Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include "NE_Solver.h"

void NE_Piston::init(void) {
	set_id(0);
	_mass = 0.;
}

void NE_Conrod::init(void) {
	set_id(0);
	_mass = 0.;
	_length = 0.;
	_cog = 0.;
}

void NE_MB::init(void) {
	set_id(0);
	_axpos = 0.;
}

void NE_Web::init(void) {
	set_id(0);
	_axpos = 0.;
	_mass = 0.;
	_radius = 0.;
	_cog = 0.;
}

void NE_CW::init(void) {
	set_id(0);
	_axpos = 0.;
	_mass = 0.;
	_radius = 0.;
	_cog = 0.;
}

void NE_Pressure::init(void) {
	set_id(0);
	_path = "";
	_angle.clear();
	_pressure.clear();
}

void NE_Pressure::set(void) {
	string path = get_path();
	if (path.empty()) {
		NE_Error_Handler::error(0);
	}
	else {
		ifstream ReadFile;
		try {			
			ReadFile.open(path);
			if (ReadFile.fail()) throw 0;
		}
		catch(int num) {
			NE_Error_Handler::error(num);
		}
		if (ReadFile.is_open()) {
			string str;
			vector<string> str_vec;
			vector<double>& angle = get_angle();
			vector<double>& pressure = get_pressure();

			while (!ReadFile.eof()) {
				str = "";
				getline(ReadFile, str);
				str_vec = NE_Utils::parsing_string(str, "\t");
				angle.push_back(stod(str_vec[0]));
				pressure.push_back(stod(str_vec[1]));
			}
		}
	}
}

void NE_Parameters::init(void) {
	set_id(0);
	_rpm = 0.;
	_ang_step = 1.;
}

void NE_Cylinder::init(void) {
	set_id(0);
	_axpos = 0.;
	_piston_id = 0;
	_conrod_id = 0;
	_pressure_id = 0;
	_crank_pin_mass = 0.;
	_piston = NULL;
	_conrod = NULL;
	_pressure = NULL;
}

void NE_Cylinder::set(NE_Solver& solver) {
	// Piston
	int id = get_piston_id();

	vector<NE_Piston*>::iterator piston_it;
	vector<NE_Piston*>& piston_ = solver.get_piston_list();
	for (piston_it = piston_.begin(); piston_it != piston_.end(); piston_it++) {
		if (id == (**piston_it).get_id()) {
			_piston = *piston_it;
			break;
		}		
	}
	// Conrod
	id = get_conrod_id();

	vector<NE_Conrod*>::iterator conrod_it;
	vector<NE_Conrod*>& conrod_ = solver.get_conrod_list();
	for (conrod_it = conrod_.begin(); conrod_it != conrod_.end(); conrod_it++) {
		if (id == (**conrod_it).get_id()) {
			_conrod = *conrod_it;
			break;
		}
	}

	// Pressure
	id = get_pressure_id();

	vector<NE_Pressure*>::iterator pressure_it;
	vector<NE_Pressure*>& pressure_ = solver.get_pressure_list();
	for (pressure_it = pressure_.begin(); pressure_it != pressure_.end(); pressure_it++) {
		if (id == (**pressure_it).get_id()) {
			_pressure = *pressure_it;
			break;
		}
	}
}

void NE_Throw::init(void) {
	set_id(0);
	_mb_id[0] = 0;
	_mb_id[1] = 0;
	_web_id[0] = 0;
	_web_id[1] = 0;
	_cw_id[0] = 0;
	_cw_id[1] = 0;
	_cyl_id.clear();
}

void NE_Throw::set(NE_Solver& solver) {
	// Cylinder
	vector<int>::iterator cyl_it;
	vector<int>& cyl_ = get_cyl_id();

	vector<NE_Cylinder*>::iterator cylinder_it;
	vector<NE_Cylinder*>& cylinder_ = solver.get_cylinder_list();
	for (cyl_it = cyl_.begin(); cyl_it != cyl_.end(); cyl_it++) {
		for (cylinder_it = cylinder_.begin(); cylinder_it != cylinder_.end(); cylinder_it++) {
			if (*(cyl_it) == (**cylinder_it).get_id()) {
				(*cylinder_it)->set(solver);
				add_cylinder(*cylinder_it);
				break;
			}
		}
	}
}

void NE_Throw::calculate(void) {
	int a = 1;

}
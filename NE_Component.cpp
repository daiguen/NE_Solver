#include "NE_Component.h"
#include "NE_Error_Handler.h"
#include "NE_Utils.h"
#include "NE_Solver.h"
#include <iostream>
#include <fstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

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
		catch (int num) {
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

	// MB & Web & CW
	int i = 0;
	int* mb_ = get_mb_id();
	int* wb_ = get_web_id();
	int* cw_ = get_cw_id();

	vector<NE_MB*>::iterator mainbearing_it;
	vector<NE_Web*>::iterator web_it;
	vector<NE_CW*>::iterator counterweight_it;
	vector<NE_MB*>& mainbearing_ = solver.get_mb_list();
	vector<NE_Web*>& web_ = solver.get_web_list();
	vector<NE_CW*>& counterweight_ = solver.get_cw_list();
	for (i = 0; i < 2; i++) {
		for (mainbearing_it = mainbearing_.begin(); mainbearing_it != mainbearing_.end(); mainbearing_it++) {
			if (mb_[i] == (**mainbearing_it).get_id()) {
				add_mb(*mainbearing_it);
				break;
			}
		}
		for (web_it = web_.begin(); web_it != web_.end(); web_it++) {
			if (wb_[i] == (**web_it).get_id()) {
				add_web(*web_it);
				break;
			}
		}
		for (counterweight_it = counterweight_.begin(); counterweight_it != counterweight_.end(); counterweight_it++) {
			if (cw_[i] == (**counterweight_it).get_id()) {
				add_cw(*counterweight_it);
				break;
			}
		}
	}
}

void NE_Throw::calculate(NE_Parameters& param) {
	double w = param.get_rpm() * M_PI / 30;
	double ang_step = param.get_angle_step();
	int n = 0;
	if (param.get_type() == "2-stroke") {
		n = int(360 / ang_step);
	}
	else if (param.get_type() == "4-stroke") {
		n = int(720 / ang_step);
	}
	else {

	}


	double m1, m2, m3;
	double i1, i2, i3;
	
	vector<NE_Web*>::iterator web_it;
	vector<NE_Web*>& web_list = get_web_list();
	for (web_it = web_list.begin(); web_it != web_list.end(); web_it++) {

	}
	vector<NE_CW*>::iterator cw_it;
	vector<NE_CW*>& cw_list = get_cw_list();
	for (cw_it = cw_list.begin(); cw_it != cw_list.end(); cw_it++) {

	}

	vector<NE_Cylinder*>::iterator cyl_it;
	vector<NE_Cylinder*>& cyl_list = get_cyl_list();
	for (cyl_it = cyl_list.begin(); cyl_it != cyl_list.end(); cyl_it++) {
		
		
		m2 = (**cyl_it).get_conrod().get_mass();
		m3 = (**cyl_it).get_piston().get_mass();
		for (int i = 0; i < n; i++) {

		}
	}
	
	

}
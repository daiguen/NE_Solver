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
			vector<double>& nangle = get_nangle();
			vector<double>& npressure = get_npressure();

			while (!ReadFile.eof()) {
				str = "";
				getline(ReadFile, str);
				str_vec = NE_Utils::parsing_string(str, "\t");
				angle.push_back(stod(str_vec[0]));
				pressure.push_back(stod(str_vec[1]));
				nangle.push_back(-stod(str_vec[0]));
				npressure.push_back(stod(str_vec[1]));
			}
			reverse(npressure.begin(), npressure.end());
			ReadFile.close();
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
	_throw_angle = 0.;
	_mb_id[0] = 0;
	_mb_id[1] = 0;
	_web_id[0] = 0;
	_web_id[1] = 0;
	_cw_id[0] = 0;
	_cw_id[1] = 0;
	_cyl_id.clear();
	_mb_list.clear();
	_web_list.clear();
	_cw_list.clear();
	_cyl_list.clear();
	_radius = 0.;
	_u.clear();
	_v.clear();
	_a.clear();
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
		double r(0.);
		for (web_it = web_.begin(); web_it != web_.end(); web_it++) {
			if (wb_[i] == (**web_it).get_id()) {
				add_web(*web_it);
				if (r == 0.) continue;
				if (r != (**web_it).get_radius()) {
					// diff radius error
				}
				break;
			}
		}
		set_radius(r);

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
	
	// Crank Total Mass & COG
	double temp(0.), temp_m(0.);
	vector<NE_Web*>::iterator web_it;
	vector<NE_Web*>& web_list = get_web_list();
	for (web_it = web_list.begin(); web_it != web_list.end(); web_it++) {
		NE_Web& web = (**web_it);
		temp_m += web.get_mass();
		temp += (web.get_mass()) * (web.get_cog());
	}
	vector<NE_CW*>::iterator cw_it;
	vector<NE_CW*>& cw_list = get_cw_list();
	for (cw_it = cw_list.begin(); cw_it != cw_list.end(); cw_it++) {
		NE_CW& cw = (**cw_it);
		temp_m += cw.get_mass();
		temp -= (cw.get_mass()) * (cw.get_cog());
	}

	double throw_angle = get_throw_angle();
	double m1 = temp_m;
	double r = temp / temp_m;
	
	vector<NE_Cylinder*>::iterator cyl_it;
	vector<NE_Cylinder*>& cyl_list = get_cyl_list();
	for (cyl_it = cyl_list.begin(); cyl_it != cyl_list.end(); cyl_it++) {						
		calculate_((**cyl_it), throw_angle, n, m1, r, w);
	}
}

void NE_Throw::calculate_(NE_Cylinder& cyl, double throw_angle, int n, double m1, double r, double w) {
	double* th1 = new double[n];
	double* y1 = new double[n];
	double* z1 = new double[n];
	double* th2 = new double[n];
	double* y2 = new double[n];
	double* z2 = new double[n];
	double* th3 = new double[n];
	double* y3 = new double[n];
	double* z3 = new double[n];

	double* th1_dot = new double[n];
	double* y1_dot = new double[n];
	double* z1_dot = new double[n];
	double* th2_dot = new double[n];
	double* y2_dot = new double[n];
	double* z2_dot = new double[n];
	double* th3_dot = new double[n];
	double* y3_dot = new double[n];
	double* z3_dot = new double[n];

	double* th1_2dot = new double[n];
	double* y1_2dot = new double[n];
	double* z1_2dot = new double[n];
	double* th2_2dot = new double[n];
	double* y2_2dot = new double[n];
	double* z2_2dot = new double[n];
	double* th3_2dot = new double[n];
	double* y3_2dot = new double[n];
	double* z3_2dot = new double[n];
	
	double m2 = cyl.get_conrod().get_mass();
	double m3 = cyl.get_piston().get_mass();
	double i2 = cyl.get_conrod().get_inertia();
	double bank_angle = cyl.get_bank_angle();

	double bb(0.);
	if (w >= 0.) bb = 1.;
	else bb = -1.;
	
	double R = get_radius() - r;
	for (int i = 0; i < n; i++) {
		th1[i] = bb * (i + throw_angle) * M_PI / 180.;
		y1[i] = cos(th1[i]) * r;
		z1[i] = sin(th1[i]) * r;
		//th2[i] = asin((-R / L) * sin(th1[i]));
	}
}
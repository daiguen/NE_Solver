#include "NE_Component.h"
#include "NE_Error_Handler.h"
#include "NE_Utils.h"
#include "NE_Solver.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

void NE_Piston::init(void) {
	set_id(0);
	_mass = 0.;
	_bore = 0.;
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

void NE_MB::set_force(int n) {
	for (int i = 0; i < n; i++) {
		_Fy.push_back(0.);
		_Fz.push_back(0.);
	}
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
	_nangle.clear();
	_npressure.clear();
	_reset_flag = false;
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

void NE_Pressure::reset(int n, double angle_step) {
	_reset_flag = true;
	if (n != 360 && n != 720) {
		vector<double>::iterator a_it;
		vector<double>::iterator p_it;
		vector<double> new_angle;
		vector<double> new_pressure;
		double ang(0.), press(0.);
		int j(0), idx1(0), idx2(0);
		for (int i = 0; i < n; i++) {
			ang = i * angle_step;
			j = 0;
			idx1 = 0;
			idx2 = 0;
			for (a_it = get_angle().begin(); a_it != get_angle().end(); a_it++) {
				if ((*a_it) == ang) {
					press = get_pressure()[j];
					break;
				}
				else if ((*a_it) < ang) {
					idx1 = j;
				}
				else if ((*a_it) > ang) {
					idx2 = j;
					press = NE_Utils::linear_interpolation(get_angle()[idx1], get_angle()[idx2], get_pressure()[idx1], get_pressure()[idx2], ang);
					break;
				}
				j++;
			}
			new_angle.push_back(ang);
			new_pressure.push_back(press);
		}
		vector<double>& angle = get_angle();
		vector<double>& pressure = get_pressure();
		vector<double>& nangle = get_nangle();
		vector<double>& npressure = get_npressure();
		angle.clear();
		pressure.clear();
		nangle.clear();
		npressure.clear();
		angle.insert(angle.end(), new_angle.begin(), new_angle.end());
		pressure.insert(pressure.end(), new_pressure.begin(), new_pressure.end());
		for (a_it = angle.begin(); a_it != angle.end(); a_it++) {
			nangle.push_back(-(*a_it));			
		}		
		npressure.insert(npressure.end(), pressure.begin(), pressure.end());
		reverse(npressure.begin(), npressure.end());
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
				if (r == 0.) {
					r = (**web_it).get_radius();
					continue;
				}
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
	double angle_step = param.get_angle_step();
	int n = 0;
	if (param.get_type() == "2") {
		n = int(360 / angle_step);
	}
	else if (param.get_type() == "4") {
		n = int(720 / angle_step);
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
	vector<NE_Cylinder*>::iterator cyl_it;
	vector<NE_Cylinder*>& cyl_list = get_cyl_list();
	for (cyl_it = cyl_list.begin(); cyl_it != cyl_list.end(); cyl_it++) {
		temp_m += (**cyl_it).get_crank_pin_mass();
		temp += (**cyl_it).get_crank_pin_mass() * get_radius();
	}	
	double m1 = temp_m;
	double r = temp / temp_m;
	
	double  mb1_axpos, mb2_axpos;
	vector<NE_MB*>& mb_list = get_mb_list();	
	mb1_axpos = mb_list[0]->get_axpos();
	mb2_axpos = mb_list[1]->get_axpos();
	
	double cyl_axpos;
	double r1;
	vector<double> S_MB_Fy, S_MB_Fz;
	vector<double>& MB_Fy1 = mb_list[0]->get_Fy();
	vector<double>& MB_Fz1 = mb_list[0]->get_Fz();
	vector<double>& MB_Fy2 = mb_list[1]->get_Fy();
	vector<double>& MB_Fz2 = mb_list[1]->get_Fz();
	int cyl_num = int(cyl_list.size());	
	for (cyl_it = cyl_list.begin(); cyl_it != cyl_list.end(); cyl_it++) {												
		calculate_((**cyl_it), angle_step, n, m1, r, w, S_MB_Fy, S_MB_Fz, cyl_num);
		cyl_axpos = (**cyl_it).get_axpos();
		r1 = abs(mb2_axpos - cyl_axpos) / abs(mb1_axpos - mb2_axpos);		
		for (int i = 0; i < n; i++) {
			MB_Fy1[i] += r1 * S_MB_Fy[i];
			MB_Fy2[i] += (1-r1) * S_MB_Fy[i];
			MB_Fz1[i] += r1 * S_MB_Fz[i];
			MB_Fz2[i] += (1 - r1) * S_MB_Fz[i];
		}		
		S_MB_Fy.clear();
		S_MB_Fz.clear();
	}	
}

void NE_Throw::calculate_(NE_Cylinder& cyl, double angle_step, int n, double m1, double r, double w, vector<double>& S_MB_Fy, vector<double>& S_MB_Fz, int cyl_num) {
	double th1, y1, z1, th2, y2, z2, th3, y3, z3;
	double th1_dot, y1_dot, z1_dot, th2_dot, y2_dot, z2_dot, th3_dot, y3_dot, z3_dot;
	double th1_2dot, y1_2dot, z1_2dot, th2_2dot, y2_2dot, z2_2dot, th3_2dot, y3_2dot, z3_2dot;
	double Fy1, Fz1, Fy2, Fz2, Fy3, Fz3;
	double T_Fy1, T_Fz1, T_Fy2, T_Fz2, T_Fy3, T_Fz3;	

	double m2 = cyl.get_conrod().get_mass();
	double m3 = cyl.get_piston().get_mass();
	double i2 = cyl.get_conrod().get_inertia();
	double bank_angle = cyl.get_bank_angle() * M_PI / 180.;
	double g = 9806.65;

	double a21(0.), b21(0.), a23(0.), b23(0.);
	double bore = cyl.get_piston().get_bore();
	double bb(0.);
	if (w >= 0.) bb = 1.;
	else bb = -1.;
	
	double R = get_radius();
	double L = cyl.get_conrod().get_length();
	double p = cyl.get_conrod().get_cog();

	if (!cyl.get_pressure().get_reset_flag()) cyl.get_pressure().reset(n, angle_step);
	vector<double>& angle = cyl.get_pressure().get_angle(w);
	vector<double>& press = cyl.get_pressure().get_pressure(w);

	vector<double> MB_Fy, MB_Fz, MB_F;

	for (int i = 0; i < n; i++) {
		// displacement
		th1 = bb * (i) * M_PI / 180.;
		y1 = cos(th1) * r;
		z1 = sin(th1) * r;
		th2 = asin((-R / L) * sin(th1));
		y2 = R * cos(th1) + p * cos(th2);
		z2 = R * sin(th1) + p * sin(th2);
		th3 = 0.;
		y3 = R * cos(th1) + L * cos(th2);
		z3 = 0.;

		// velocity
		th1_dot = w;
		y1_dot = -r * th1_dot * sin(th1);
		z1_dot = r * th1_dot * cos(th1);
		th2_dot = (-R / L) * (cos(th1) / cos(th2)) * th1_dot;
		y2_dot = -R * th1_dot * sin(th1) - p * th2_dot * sin(th2);
		z2_dot = R * th1_dot * cos(th1) + p * th2_dot * cos(th2);
		th3_dot = 0.;
		y3_dot = -R * th1_dot * sin(th1) - L * th2_dot * sin(th2);
		z3_dot = 0.;

		// acceleration
		th1_2dot = 0.;
		y1_2dot = -r * th1_dot * th1_dot * cos(th1);
		z1_2dot = -r * th1_dot * th1_dot * sin(th1);
		th2_2dot = ((R / L) * th1_dot * th1_dot * sin(th1) + th2_dot * th2_dot * sin(th2)) / cos(th2);
		y2_2dot = -R * th1_dot * th1_dot * cos(th1) - p * th2_2dot * sin(th2) - p * th2_dot * th2_dot * cos(th2);
		z2_2dot = -R * th1_dot * th1_dot * sin(th1) + p * th2_2dot * cos(th2) - p * th2_dot * th2_dot * sin(th2);
		th3_2dot = 0.;
		y3_2dot = -R * th1_dot * th1_dot * cos(th1) - L * th2_2dot * sin(th2) - L * th2_dot * th2_dot * cos(th2);
		z3_2dot = 0.;

		// reaction force
		Fy3 = m3 * y3_2dot + 1000. * 0.1 * press[i] * M_PI * bore * bore * 0.25  + m3 * g * cos(bank_angle);
		Fy2 = m2 * y2_2dot + Fy3 + m2 * g * cos(bank_angle);

		a21 = cos(th2) * -p;
		b21 = sin(th2) * -p;
		a23 = cos(th2) * (L - p);
		b23 = sin(th2) * (L - p);

		Fz3 = (i2 * th2_2dot + b21 * Fy2 - b23 * Fy3 + a21 * m2 * g * sin(bank_angle) - a21 * m2 * z2_2dot ) / (a21 - a23);
		Fz2 = Fz3 + m2 * z2_2dot - m2 * g * sin(bank_angle);

		//Fy1 = Fy2;
		//Fz1 = Fz2;
		double ratio(0.);
		if (cyl_num == 0) {

		}
		else {
			ratio = 1. / cyl_num;
		}

		Fy1 = ratio * m1 * y1_2dot + Fy2 + ratio * m1 * g * cos(bank_angle);
		Fz1 = ratio * m1 * z1_2dot + Fz2 - ratio * m1 * g * sin(bank_angle);
	
		// Transformation by bank angle
		T_Fy1 = cos(bank_angle)* Fy1 - sin(bank_angle) * Fz1;
		T_Fz1 = sin(bank_angle) * Fy1 + cos(bank_angle) * Fz1;
		T_Fy2 = cos(bank_angle) * Fy2 - sin(bank_angle) * Fz2;
		T_Fz2 = sin(bank_angle) * Fy2 + cos(bank_angle) * Fz2;
		T_Fy3 = cos(bank_angle) * Fy3 - sin(bank_angle) * Fz3;
		T_Fz3 = sin(bank_angle) * Fy3 + cos(bank_angle) * Fz3;

		// Save
		T_Fy1 = -T_Fy1 * 0.000000001;
		T_Fz1 = -T_Fz1 * 0.000000001;
		MB_Fy.push_back(T_Fy1);
		MB_Fz.push_back(T_Fz1);
	}
	// Shift
	int idx(0);
	double throw_angle = get_throw_angle();
	double firing_angle = cyl.get_firing_angle();
	double shift_angle = abs(firing_angle);
	idx = n - int(shift_angle / angle_step);

	S_MB_Fy.insert(S_MB_Fy.end(), MB_Fy.begin() + idx, MB_Fy.end());
	S_MB_Fz.insert(S_MB_Fz.end(), MB_Fz.begin() + idx, MB_Fz.end());
	if (idx != 0) {
		S_MB_Fy.insert(S_MB_Fy.end(), MB_Fy.begin(), MB_Fy.begin() + idx);
		S_MB_Fz.insert(S_MB_Fz.end(), MB_Fz.begin(), MB_Fz.begin() + idx);
	}

	//double max_yval = *max_element(S_MB_Fy.begin(), S_MB_Fy.end());
	//double max_zval = *max_element(S_MB_Fz.begin(), S_MB_Fz.end());	
}
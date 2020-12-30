#include "NE_Input_Reader.h"
#include "NE_Domain.h"
#include "NE_Utils.h"
#include "NE_Component.h"
#include "NE_Solver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void NE_Input_Reader::read(NE_Solver& solver) {
	ifstream ReadFile;
	ReadFile.open(get_input_file());

	if (ReadFile.is_open()) {
		string str, key;
		vector<string> str_vec, prev_str_vec;

		int count = 0;
		while (!ReadFile.eof()) {
			str = "";
			getline(ReadFile, str);
			str_vec = NE_Utils::parsing_string(str, "\t");
			// empty line
			if (str_vec.empty()) continue;

			if (count == 0) {
				prev_str_vec = str_vec;
				count++;
			}
			else {
				key = str_vec[0];
				if (key.empty()) {
					prev_str_vec.insert(prev_str_vec.end(), str_vec.begin() + 1, str_vec.end());
				}
				else {
					read_(solver, prev_str_vec);
					prev_str_vec = str_vec;
				}
				count++;
			}
		}
		read_(solver, prev_str_vec);
		ReadFile.close();
	}
}

void NE_Input_Reader::read_(NE_Solver& solver, vector<string>& str_vec) {
	string key = str_vec[0];	
	
	if (key == "DOMAIN") {
		NE_Domain& dom = solver.get_domain();
		dom.set_id(stoi(str_vec[1]));
		dom.set_param_id(stoi(str_vec[2]));
		int n = int(str_vec.size());
		for (int i = 3; i < n; i++) {
			dom.set_throw_id(stoi(str_vec[i]));
		}
	}
	else if (key == "PARAMETERS") {
		NE_Parameters* ne_param = new NE_Parameters;
		ne_param->set_id(stoi(str_vec[1]));
		ne_param->set_rpm(stod(str_vec[2]));
		ne_param->set_angle_step(stod(str_vec[3]));
		ne_param->set_type(str_vec[4]);
		solver.add_parameter(ne_param);
	}
	else if (key == "THROW") {
		NE_Throw* ne_throw = new NE_Throw;
		ne_throw->set_id(stoi(str_vec[1]));
		ne_throw->set_mb_id(stoi(str_vec[2]), stoi(str_vec[3]));
		ne_throw->set_web_id(stoi(str_vec[4]), stoi(str_vec[5]));
		ne_throw->set_cw_id(stoi(str_vec[6]), stoi(str_vec[7]));
		int n = int(str_vec.size());
		for (int i = 8; i < n; i++) {
			ne_throw->set_cyl_id(i);
		}
		solver.add_throw(ne_throw);
	}
	else if (key == "CYLINDER") {
		NE_Cylinder* ne_cyl = new NE_Cylinder;
		ne_cyl->set_id(stoi(str_vec[1]));
		ne_cyl->set_bank_angle(stod(str_vec[2]));
		ne_cyl->set_axpos(stod(str_vec[3]));
		ne_cyl->set_firing_angle(stod(str_vec[4]));
		ne_cyl->set_piston_id(stoi(str_vec[5]));
		ne_cyl->set_conrod_id(stoi(str_vec[6]));
		ne_cyl->set_pressure_id(stoi(str_vec[7]));
		ne_cyl->set_crank_pin_mass(stod(str_vec[8]));
		solver.add_cylinder(ne_cyl);
	}
	else if (key == "PISTON") {
		NE_Piston* ne_piston = new NE_Piston;
		ne_piston->set_id(stoi(str_vec[1]));
		ne_piston->set_mass(stod(str_vec[2]));
		solver.add_piston(ne_piston);
	}
	else if (key == "CONROD") {
		NE_Conrod* ne_conrod = new NE_Conrod;
		ne_conrod->set_id(stoi(str_vec[1]));
		ne_conrod->set_mass(stod(str_vec[2]));
		ne_conrod->set_inertia(stod(str_vec[3]));
		ne_conrod->set_length(stod(str_vec[4]));
		ne_conrod->set_cog(stod(str_vec[5]));
		solver.add_conrod(ne_conrod);
	}
	else if (key == "MB") {
		NE_MB* ne_mb = new NE_MB;
		ne_mb->set_id(stoi(str_vec[1]));
		ne_mb->set_axpos(stod(str_vec[2]));
		solver.add_mb(ne_mb);
	}
	else if (key == "WEB") {
		NE_Web* ne_web = new NE_Web;
		ne_web->set_id(stoi(str_vec[1]));
		ne_web->set_axpos(stod(str_vec[2]));
		ne_web->set_mass(stod(str_vec[3]));
		ne_web->set_radius(stod(str_vec[4]));
		ne_web->set_cog(stod(str_vec[5]));
		solver.add_web(ne_web);
	}
	else if (key == "CW") {
		NE_CW* ne_cw = new NE_CW;
		ne_cw->set_id(stoi(str_vec[1]));
		ne_cw->set_axpos(stod(str_vec[2]));
		ne_cw->set_mass(stod(str_vec[3]));
		ne_cw->set_radius(stod(str_vec[4]));
		ne_cw->set_cog(stod(str_vec[5]));
		solver.add_cw(ne_cw);
	}
	else if (key == "PRESSURE") {
		NE_Pressure* ne_press = new NE_Pressure;
		ne_press->set_id(stoi(str_vec[1]));
		ne_press->set_path(str_vec[2]);
		ne_press->set();
		solver.add_pressure(ne_press);		
	}
	else {
		// KEY ERROR
	}

}
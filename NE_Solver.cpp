#include "NE_Solver.h"
#include "NE_Input_Reader.h"
#include "NE_Domain.h"
#include "NE_Component.h"
#include "NE_Utils.h"
#include <iostream>
#include <fstream>

void NE_Solver::set(string inp) {
	vector<string> str_vec;
	str_vec = NE_Utils::parsing_string(inp, "\\");
	vector<string>::iterator str_vec_it;
	string path = "";
	string fname;
	for (str_vec_it = str_vec.begin(); str_vec_it != str_vec.end() - 1; str_vec_it++) {
		path += *str_vec_it + "\\";
	}
	vector<string> str_vec2;
	str_vec2 = NE_Utils::parsing_string((*str_vec_it), ".");
	fname = str_vec2[0];
	set_path(path);
	set_file_name(fname);

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
	
	double angle_step = dom.get_parameters().get_angle_step();
	int n = 0;
	if (dom.get_parameters().get_type() == "2-stroke") {
		n = int(360 / angle_step);
	}
	else if (dom.get_parameters().get_type() == "4-stroke") {
		n = int(720 / angle_step);
	}
	else {

	}
	for (throw_it = throw_.begin(); throw_it != throw_.end(); throw_it++) {
		vector<NE_MB*>& mb_list = (**throw_it).get_mb_list();
		mb_list[0]->set_force(n);
		mb_list[1]->set_force(n);
	}

	for (throw_it = throw_.begin(); throw_it != throw_.end(); throw_it++) {
		(**throw_it).calculate(dom.get_parameters());
	}

	string ofile_name = get_path() + get_file_name() + ".out";
	ofstream ofile(ofile_name);
	if (ofile.is_open()) {
		vector<NE_Throw*>& throw_list = dom.get_throw_list();
		vector<NE_Throw*>::iterator throw_it;
		vector<NE_MB*> total_mb_list;
		vector<NE_MB*>::iterator mb_it;
		for (throw_it = throw_list.begin(); throw_it != throw_list.end(); throw_it++) {
			vector<NE_MB*>& mb_list = (**throw_it).get_mb_list();
			for (int i = 0; i < 2; i++) {
				int id = mb_list[i]->get_id();
				bool flag(false);
				for (mb_it = total_mb_list.begin(); mb_it != total_mb_list.end(); mb_it++) {
					if ((**mb_it).get_id() == id) {
						flag = true;
						break;
					}
				}
				if (!flag) total_mb_list.push_back(mb_list[i]);
			}
		}
		// Write		
		for (int i = 0; i < n; i++) {
			ofile << i * angle_step << "\t";
			for (mb_it = total_mb_list.begin(); mb_it != total_mb_list.end() - 1; mb_it++) {
				vector<double>& Fy = (**mb_it).get_Fy();
				vector<double>& Fz = (**mb_it).get_Fz();
				ofile << Fy[i] << "\t";
				ofile << Fz[i] << "\t";
				ofile << sqrt(Fy[i] * Fy[i] + Fz[i] * Fz[i]) << "\t";
			}
			vector<double>& Fy = (**mb_it).get_Fy();
			vector<double>& Fz = (**mb_it).get_Fz();
			ofile << Fy[i] << "\t";
			ofile << Fz[i] << "\t";
			ofile << sqrt(Fy[i] * Fy[i] + Fz[i] * Fz[i]) << endl;
		}		
		ofile.close();
	}

}
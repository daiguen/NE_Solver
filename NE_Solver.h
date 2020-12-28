#ifndef __NE_SOLVER_H__
#define __NE_SOLVER_H__

#include <string>
#include <vector>

using namespace std;

class NE_Input_Reader;
class NE_Domain;
class NE_Component;
class NE_Throw;
class NE_Cylinder;
class NE_Piston;
class NE_Conrod;
class NE_MB;
class NE_Web;
class NE_CW;
class NE_Pressure;
class NE_Parameters;

class NE_Solver {
private:
	NE_Input_Reader* _input_reader;
	NE_Domain* _dom;
	vector<NE_Throw*> _throw_list;
	vector<NE_Cylinder*> _cyl_list;
	vector<NE_Piston*> _piston_list;
	vector<NE_Conrod*> _conrod_list;
	vector<NE_MB*> _mb_list;
	vector<NE_Web*> _web_list;
	vector<NE_CW*> _cw_list;
	vector<NE_Pressure*> _press_list;
	vector<NE_Parameters*> _param_list;

public:
	NE_Solver() {
		_input_reader = NULL;
		_dom = NULL;
	}
	~NE_Solver() {

	}
	// set functions
	void set(string inp);	

	// get functions
	NE_Input_Reader& get_input_reader(void) { return *_input_reader; }
	NE_Domain& get_domain(void) { return *_dom; }
	vector<NE_Throw*>& get_throw_list(void) { return _throw_list; }
	vector<NE_Cylinder*>& get_cylinder_list(void) { return _cyl_list; }
	vector<NE_Piston*>& get_piston_list(void) { return _piston_list; }
	vector<NE_Conrod*>& get_conrod_list(void) { return _conrod_list; }
	vector<NE_MB*>& get_mb_list(void) { return _mb_list; }
	vector<NE_Web*>& get_web_list(void) { return _web_list; }
	vector<NE_CW*>& get_cw_list(void) { return _cw_list; }
	vector<NE_Pressure*>& get_pressure_list(void) { return _press_list; }
	vector<NE_Parameters*>& get_parameters_list(void) { return _param_list; }

	// non-virtual functions
	void run(void);
	void add_throw(NE_Throw* ne_throw) { _throw_list.push_back(ne_throw); }
	void add_cylinder(NE_Cylinder* ne_cyl) { _cyl_list.push_back(ne_cyl); }
	void add_piston(NE_Piston* ne_piston) { _piston_list.push_back(ne_piston); }
	void add_conrod(NE_Conrod* ne_conrod) { _conrod_list.push_back(ne_conrod); }
	void add_mb(NE_MB* ne_mb) { _mb_list.push_back(ne_mb); }
	void add_web(NE_Web* ne_web) { _web_list.push_back(ne_web); }
	void add_cw(NE_CW* ne_cw) { _cw_list.push_back(ne_cw); }
	void add_pressure(NE_Pressure* ne_press) { _press_list.push_back(ne_press); }
	void add_parameter(NE_Parameters* ne_param) { _param_list.push_back(ne_param); }
};


#endif
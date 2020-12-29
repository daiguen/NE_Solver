#ifndef __NE_COMPONENT_H__
#define __NE_COMPONENT_H__

#include <vector>
#include <string>

using namespace std;

class NE_Solver;

class NE_Component {
private:
	int _id;

public:
	NE_Component() {
		_id = 0;
	}
	~NE_Component() {

	}

	// set functions
	void set_id(int id) { _id = id; }
	
	// get functions
	int get_id(void) { return _id; }

	// virtual functions
	virtual void init(void) {}

	// non-virtual functions


	
};

class NE_Piston : public NE_Component {
private:
	double _mass;

public:
	NE_Piston() {
		init();
	}
	~NE_Piston() {

	}
	// set functions
	void set_mass(double mass) { _mass = mass; }	

	// get functions
	double get_mass(void) { return _mass; }
	
	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_Conrod : public NE_Component {
private:
	double _mass;
	double _length;
	double _cog;

public:
	NE_Conrod() {
		init();
	}
	~NE_Conrod() {

	}
	// set functions
	void set_mass(double mass) { _mass = mass; }
	void set_length(double length) { _length = length; }
	void set_cog(double cog) { _cog = cog; }

	// get functions
	double get_mass(void) { return _mass; }
	double get_length(void) { return _length; }
	double get_cog(void) { return _cog; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_MB : public NE_Component {
private:
	double _axpos;

public:
	NE_MB() {
		init();
	}
	~NE_MB() {

	}
	// set functions
	void set_axpos(double axpos) { _axpos = axpos; }
	
	// get functions
	double get_axpos(void) { return _axpos; }

	// virtual functions
	void init(void);

	// non-virtual functions

};

class NE_Web : public NE_Component {
private:
	double _axpos;
	double _mass;
	double _radius;
	double _cog;

public:
	NE_Web() {
		init();
	}
	~NE_Web() {

	}
	// set functions
	void set_axpos(double axpos) { _axpos = axpos; }
	void set_mass(double mass) { _mass = mass; }
	void set_radius(double radius) { _radius = radius; }
	void set_cog(double cog) { _cog = cog; }

	// get functions
	double get_axpos(void) { return _axpos; }
	double get_mass(void) { return _mass; }
	double get_radius(void) { return _radius; }
	double get_cog(void) { return _cog; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_CW : public NE_Component {
private:
	double _axpos;
	double _mass;
	double _radius;
	double _cog;

public:
	NE_CW() {
		init();
	}
	~NE_CW() {

	}
	// set functions
	void set_axpos(double axpos) { _axpos = axpos; }
	void set_mass(double mass) { _mass = mass; }
	void set_radius(double radius) { _radius = radius; }
	void set_cog(double cog) { _cog = cog; }

	// get functions
	double get_axpos(void) { return _axpos; }
	double get_mass(void) { return _mass; }
	double get_radius(void) { return _radius; }
	double get_cog(void) { return _cog; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_Pressure : public NE_Component {
private:
	string _path;
	vector<double> _angle;
	vector<double> _pressure;

public:
	NE_Pressure() {
		init();
	}
	~NE_Pressure() {

	}
	// set functions
	void set_path(string path) { _path = path; }
	void set(void);

	// get functions
	string get_path(void) { return _path; }
	vector<double>& get_angle(void) { return _angle; }
	vector<double>& get_pressure(void) { return _pressure; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_Parameters : public NE_Component {
private:
	double _rpm;
	double _ang_step;
	string _type;

public:
	NE_Parameters() {		
		init();
	}
	~NE_Parameters() {

	}
	// set functions
	void set_rpm(double rpm) { _rpm = rpm; }
	void set_angle_step(double ang_step) { _ang_step = ang_step; }
	void set_type(string type) { _type = type; }

	// get functions
	double get_rpm(void) { return _rpm; }
	double get_angle_step(void) { return _ang_step; }
	string get_type(void) { return _type; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_Cylinder : public NE_Component {
private:
	double _axpos;
	int _piston_id;
	int _conrod_id;
	int _pressure_id;
	double _crank_pin_mass;

	NE_Piston* _piston;
	NE_Conrod* _conrod;
	NE_Pressure* _pressure;

public:
	NE_Cylinder() {
		init();
	}
	~NE_Cylinder() {

	}
	// set functions
	void set_axpos(double axpos) { _axpos = axpos; }
	void set_piston_id(int id) { _piston_id = id; }
	void set_conrod_id(int id) { _conrod_id = id; }
	void set_pressure_id(int id) { _pressure_id = id; }
	void set_crank_pin_mass(double mass) { _crank_pin_mass = mass; }
	void set(NE_Solver& solver);

	// get functions
	double get_axpos(void) { return _axpos; }
	int get_piston_id(void) { return _piston_id; }
	int get_conrod_id(void) { return _conrod_id; }
	int get_pressure_id(void) { return  _pressure_id; }
	double get_crank_pin_mass(void) { return _crank_pin_mass; }

	// virtual functions
	void init(void);

	// non-virtual functions
};

class NE_Throw : public NE_Component {
private:
	int _mb_id[2];
	int _web_id[2];
	int _cw_id[2];
	vector<int> _cyl_id;

	vector<NE_MB*> _mb_list;
	vector<NE_Web*> _web_list;
	vector<NE_CW*> _cw_list;
	vector<NE_Cylinder*> _cyl_list;

public:
	NE_Throw() {
		init();
	}
	~NE_Throw() {

	}

	// set functions
	void set_mb_id(int id1, int id2) { _mb_id[0] = id1; _mb_id[1] = id2; }
	void set_web_id(int id1, int id2) { _web_id[0] = id1; _web_id[1] = id2; }
	void set_cw_id(int id1, int id2) { _cw_id[0] = id1; _cw_id[1] = id2; }
	void set_cyl_id(int id) { _cyl_id.push_back(id); }
	void set(NE_Solver& solver);

	// get functions
	int* get_mb_id(void) { return _mb_id; }
	int* get_web_id(void) { return _web_id; }
	int* get_cw_id(void) { return _cw_id; }
	vector<int>& get_cyl_id(void) { return _cyl_id; }

	// virtual functions
	void init(void);

	// non-virtual functions	
	void calculate(NE_Parameters& param);
	void add_mb(NE_MB* mb) { _mb_list.push_back(mb); }
	void add_web(NE_Web* web) { _web_list.push_back(web); }
	void add_cw(NE_CW* cw) { _cw_list.push_back(cw); }
	void add_cylinder(NE_Cylinder* cyl) { _cyl_list.push_back(cyl); }
};

#endif
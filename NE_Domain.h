#ifndef __NE_DOMAIN_H__
#define __NE_DOMAIN_H__

#include <vector>

using namespace std;

class NE_Solver;
class NE_Throw;
class NE_Domain {
private:
	int _id;
	int _param_id;
	vector<int> _throw_id;

	vector<NE_Throw*> _throw_list;

public:
	NE_Domain() {
		init();
	}
	~NE_Domain() {

	}
	
	// set functions
	void set(NE_Solver& solver);
	void set_id(int id) { _id = id; }
	void set_param_id(int id) { _param_id = id; }
	void set_throw_id(int id) { _throw_id.push_back(id); }

	// get functions
	int get_id(void) { return _id; }
	int get_param_id(void) { return _param_id; }
	vector<int>& get_throw_id(void) { return _throw_id; }
	vector<NE_Throw*>& get_throw_list(void) { return _throw_list; }

	// non-virtual functions
	void init(void);
	void add_throw(NE_Throw* th) { _throw_list.push_back(th); }



};



#endif
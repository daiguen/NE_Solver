#ifndef __NE_INPUT_READER_H__
#define __NE_INPUT_READER_H__

#include <string>
#include <vector>

using namespace std;

class NE_Domain;
class NE_Solver;

class NE_Input_Reader {
private:
	string _inp_file;

public:
	NE_Input_Reader() {

	}
	~NE_Input_Reader() {

	}

	// set functions
	void set(string inp) { _inp_file = inp; }

	// get functions
	string get_input_file(void) { return _inp_file; }

	// non-virtual functions
	void read(NE_Solver& solver);
	void read_(NE_Solver& solver, vector<string>& str_vec);
};







#endif
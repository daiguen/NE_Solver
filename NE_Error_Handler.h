#ifndef __NE_ERROR_HANDLER_H__
#define __NE_ERROR_HANDLER_H__

#include <iostream>

using namespace std;

namespace NE_Error_Handler {
	static void error(int num) {
		if (num == 0) {
			cout << "Can't open file" << endl;
		}
		else {
			// add key
		}
		
	}
}

#endif

#ifndef __NE_UTILS_H__
#define __NE_UTILS_H__

#include <string>
#include <vector>

using namespace std;

namespace NE_Utils {
	static vector<string> parsing_string(string text, string delimeter) {
		vector<string> parsing;

		string tmp_text = text;
		while (tmp_text.length()) {
			int pos = int(tmp_text.find(delimeter));
			string  fword = (pos >= 0 ? tmp_text.substr(0, pos) : tmp_text);
			tmp_text = (pos >= 0 ? tmp_text.substr(pos + 1) : "");
			parsing.push_back(fword);
		}

		return parsing;
	}
}

#endif

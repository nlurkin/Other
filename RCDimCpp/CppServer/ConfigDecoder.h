/*
 * ConfigDecoder.h
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#ifndef CONFIGDECODER_H_
#define CONFIGDECODER_H_

#include <string>
#include <vector>
using namespace std;

class ConfigDecoder {
public:
	ConfigDecoder();
	virtual ~ConfigDecoder();

	void parseFile(string content);

	static const vector<string> tokenize(string s, const char delim = ' ');

	int param1;
	int param2;
	int param3;
	double param4;
	string param5;

private:
	void decodeLine(string line);
};

#endif /* CONFIGDECODER_H_ */

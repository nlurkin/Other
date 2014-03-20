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

class ConfigDecoder {
public:
	ConfigDecoder();
	virtual ~ConfigDecoder();

	void parseFile(std::string content);

	static const std::vector<std::string> tokenize(std::string s, const char delim = ' ');

	int param1;
	int param2;
	int param3;
	double param4;
	std::string param5;

private:
	void decodeLine(std::string line);
};

#endif /* CONFIGDECODER_H_ */

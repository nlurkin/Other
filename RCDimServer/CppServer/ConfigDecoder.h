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
#include "TestNode.h"


/*struct decoderStruct_t{
	int param1;
	int param2;
	int param3;
	double param4;
	std::string param5;
};*/

class ConfigDecoder {
public:
	ConfigDecoder();
	virtual ~ConfigDecoder();

	void parseFile(std::string content, TestNode *s);
	void writeFile(std::string content, TestNode *s);

	static const std::vector<std::string> tokenize(std::string s, const char delim = ' ');

private:
	//void decodeLine(std::string line, TestNode *s);
};

#endif /* CONFIGDECODER_H_ */

/*
 * ConfigDecoder.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include "ConfigDecoder.h"
#include <iostream>
#include <iterator>

#include <algorithm>
#include <sstream>

ConfigDecoder::ConfigDecoder(){
	param1 = -1;
	param2 = -1;
	param3 = -1;
	param4 = -1.;
	param5 = "";
}

ConfigDecoder::~ConfigDecoder() {
}

void ConfigDecoder::parseFile(std::string content){
	std::vector<std::string>::iterator it;

	std::vector<std::string> lines = tokenize(content, '\n');

	for(it=lines.begin(); it!=lines.end(); it++){
		decodeLine((*it));
	}
}

void ConfigDecoder::decodeLine(std::string line){
	std::vector<std::string> elem = tokenize(line, '=');

	if(elem.size()!=2) return;
	if(elem[0]=="uselessInt") param1 = atoi(elem[1].c_str());
	if(elem[0]=="param") param2 = atoi(elem[1].c_str());
	if(elem[0]=="sourceID") param3 = strtol(elem[1].c_str(), NULL, 16);
	if(elem[0]=="frequency") param4 = atof(elem[1].c_str());
	if(elem[0]=="uselessString") param5 = elem[1];
}

const std::vector<std::string> ConfigDecoder::tokenize(std::string s, const char delim) {
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string item;

	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}

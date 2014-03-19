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
using namespace std;

ConfigDecoder::ConfigDecoder(){
	param1 = -1;
	param2 = -1;
	param3 = -1;
	param4 = -1.;
	param5 = "";
}

ConfigDecoder::~ConfigDecoder() {
}

void ConfigDecoder::parseFile(string content){
	vector<string>::iterator it;

	vector<string> lines = tokenize(content, '\n');

	for(it=lines.begin(); it!=lines.end(); it++){
		decodeLine((*it));
	}
}

void ConfigDecoder::decodeLine(string line){
	vector<string> elem = tokenize(line, '=');

	if(elem.size()!=2) return;
	if(elem[0]=="uselessInt") param1 = atoi(elem[1].c_str());
	if(elem[0]=="param") param2 = atoi(elem[1].c_str());
	if(elem[0]=="sourceID") param3 = strtol(elem[1].c_str(), NULL, 16);
	if(elem[0]=="frequency") param4 = atof(elem[1].c_str());
	if(elem[0]=="uselessString") param5 = elem[1];
}

const vector<string> ConfigDecoder::tokenize(string s, const char delim) {
	vector<string> tokens;
	stringstream ss(s);
	string item;

	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}

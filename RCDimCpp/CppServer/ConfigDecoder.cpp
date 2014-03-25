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
#include <fstream>
#include "xmlconfig_TestNode.h"

ConfigDecoder::ConfigDecoder(){
}

ConfigDecoder::~ConfigDecoder() {
}

void ConfigDecoder::parseFile(std::string content, TestNode *s){
	std::ofstream myfile;
	myfile.open ("receivedfile.xml");
	myfile << content;
	myfile.close();
	inxmlfile_TestNode(s,"toto", "receivedfile.xml");
}

void ConfigDecoder::writeFile(std::string content, TestNode *s){
	outxmlfile_TestNode((void*)s, "toto", (char*)content.c_str());
}

/*void ConfigDecoder::decodeLine(std::string line, TestNode *s){
	std::vector<std::string> elem = tokenize(line, '=');

	if(elem.size()!=2) return;
	if(elem[0]=="uselessInt") s->param1 = atoi(elem[1].c_str());
	if(elem[0]=="param") s->param2 = atoi(elem[1].c_str());
	if(elem[0]=="sourceID") s->param3 = strtol(elem[1].c_str(), NULL, 16);
	if(elem[0]=="frequency") s->param4 = atof(elem[1].c_str());
	if(elem[0]=="uselessString") s->param5 = elem[1];
}*/

const std::vector<std::string> ConfigDecoder::tokenize(std::string s, const char delim) {
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string item;

	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TestNode.hpp"
#include "xmlconfig_TestNode.h"
using namespace std;

void outxml_TestNode(void* top_struct, char* varname){
    TestNode* topcpp_ = (TestNode*)top_struct;  //cpp class overlay over  the c struct
    TestNode & topcpp=* topcpp_;
    string xml_filename(varname);
    xml_filename.append(".xml");
    std::ofstream ofs(xml_filename.c_str());
    boost::archive::xml_oarchive oa(ofs,7);
 	oa<<boost::serialization::make_nvp(varname, topcpp);
}


void outxmlfile_TestNode(void* top_struct, char* varname, char* filename){
    TestNode* topcpp_ = (TestNode*)top_struct;  //cpp class overlay over  the c struct
    TestNode & topcpp=* topcpp_;
    string xml_filename(filename);
    std::ofstream ofs(xml_filename.c_str());
    boost::archive::xml_oarchive oa(ofs,7);
 	oa<<boost::serialization::make_nvp(varname, topcpp);
}


void inxml_TestNode(void* top_struct, char* varname){
    TestNode* topcpp_ = (TestNode*)top_struct;  //cpp class overlay over  the c struct
    TestNode & topcpp=* topcpp_;
    string xml_filename(varname);
    xml_filename.append(".xml");
    //if(sizeof(*detcpp)!= sizeof(*det)) printf("cpp over c overlay ERROR !!!!!\n");
    std::ifstream ifs(xml_filename.c_str());
    boost::archive::xml_iarchive ia(ifs,7);
    ia >> boost::serialization::make_nvp(varname, topcpp);
}
void inxmlfile_TestNode(void* top_struct, char* varname, char* filename){
    TestNode* topcpp_ = (TestNode*)top_struct;  //cpp class overlay over  the c struct
    TestNode & topcpp=* topcpp_;
    string xml_filename(filename);
    //if(sizeof(*detcpp)!= sizeof(*det)) printf("cpp over c overlay ERROR !!!!!\n");
    std::ifstream ifs(xml_filename.c_str());
    boost::archive::xml_iarchive ia(ifs,7);
    ia >> boost::serialization::make_nvp(varname, topcpp);
}

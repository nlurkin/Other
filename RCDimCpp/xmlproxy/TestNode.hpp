/*
 * TestNode.h
 *
 *  Created on: Mar 20, 2014
 *      Author: nlurkin
 */

#ifndef TESTNODE_H_
#define TESTNODE_H_

#include "xmlstring.hpp"

#define Mynode_VERSION 100


class TestNode{

public:

	int version; /*jj*/

	int param1;
	int param2;
	int param3;
	double param4;
	xmlstring param5;
	template<class Archive> void serialize(Archive & ar,const unsigned int __version) {
		ar & BOOST_SERIALIZATION_NVP(version);
		ar & BOOST_SERIALIZATION_NVP(param1);
		ar & BOOST_SERIALIZATION_NVP(param2);
		ar & BOOST_SERIALIZATION_NVP(param3);
		ar & BOOST_SERIALIZATION_NVP(param4);
		ar & BOOST_SERIALIZATION_NVP(param5);
	}
	TestNode(){
	}
};
BOOST_CLASS_IMPLEMENTATION(TestNode, object_serializable)

#endif /* TESTNODE_H_ */

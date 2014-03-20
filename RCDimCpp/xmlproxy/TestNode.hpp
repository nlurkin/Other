/*
 * TestNode.h
 *
 *  Created on: Mar 20, 2014
 *      Author: nlurkin
 */

#ifndef TESTNODE_H_
#define TESTNODE_H_

#include "xmlstring.hpp"

#define TestNode_VERSION 100


class TestNode{

public:

	int version; /*jj*/

	int uselessInt;
	int param;
	int sourceID;
	double frequency;
	xmlstring uselessString;
	template<class Archive> void serialize(Archive & ar,const unsigned int __version) {
		ar & BOOST_SERIALIZATION_NVP(version);
		ar & BOOST_SERIALIZATION_NVP(uselessInt);
		ar & BOOST_SERIALIZATION_NVP(param);
		ar & BOOST_SERIALIZATION_NVP(sourceID);
		ar & BOOST_SERIALIZATION_NVP(frequency);
		ar & BOOST_SERIALIZATION_NVP(uselessString);
	}
	TestNode(){
	}
};
BOOST_CLASS_IMPLEMENTATION(TestNode, object_serializable)

#endif /* TESTNODE_H_ */

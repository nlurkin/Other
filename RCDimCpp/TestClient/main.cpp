#include "TestDimClient.h"
/*
 * mainClient.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

using namespace std;

int main(int argc, char** argv){
	TestDimClient client("dimExample");

	client.startrun();
	client.initialize();

	client.startrun();

	while(true){

	}
}

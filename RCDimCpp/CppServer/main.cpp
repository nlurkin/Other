#include "TestServer.h"

using namespace std;

int main(int argc, char** argv){
	TestServer server("dimExample");

	server.start();
	while(1){
		server.mainLoop();
	}
}

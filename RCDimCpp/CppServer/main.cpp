#include "TestServer.h"

int main(int argc, char** argv){
	TestServer server("dimExample", 0x13);

	server.start();
	while(1){
		server.mainLoop();
	}
}

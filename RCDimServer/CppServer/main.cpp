#include "TestServer.h"

int main(int argc, char** argv){
	std::string serverName = "dimExample";
	if(argc>1) serverName = argv[1];
	TestServer server(serverName, 0x13);

	server.start();
	while(1){
		server.mainLoop();
	}
}

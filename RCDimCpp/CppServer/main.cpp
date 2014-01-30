#include "NA62DimServer.h"
#include "NA62DimCommands.h"

using namespace std;

int main(int argc, char** argv){
	NA62DimServer server("dimExample");

	server.start();
	while(1){
		server.mainLoop();
	}
}

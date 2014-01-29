#include "dis.hxx"
#include <string>
#include <iostream>

using namespace std;

class Command: public DimCommand
{
	void commandHandler(){
		cout << "Received:" << getString() << endl;
	}
public:
	Command(string dimServerName): DimCommand((dimServerName + "/Command").c_str(), "C"){};
};

class FileContent: public DimCommand
{
	void commandHandler(){
		cout << "Received:" << getString() << endl;
	}
public:
	FileContent(string dimServerName): DimCommand((dimServerName + "/FileContent").c_str(), "C"){};
};

class EndTransfer: public DimCommand
{
	void commandHandler(){
		cout << "Received:" << getInt() << endl;
	}
public:
	EndTransfer(string dimServerName): DimCommand((dimServerName + "/EndTransfer").c_str(), "I"){};
};

void mainLoop(){

}

int main(int argc, char** argv){
	string dimServerName = "exampleServer";

	int state, waiting;
	char info[100], logging[100];
	char config[1000];

	DimService dimState((dimServerName + "/State").c_str(), state);
	DimService dimInfo((dimServerName + "/Info").c_str(), info);
	DimService dimLogging((dimServerName + "/Logging").c_str(), logging);
	DimService dimWaiting((dimServerName + "/Waiting").c_str(), waiting);
	DimService dimConfig((dimServerName + "/Config").c_str(), config);

	Command dimCommand(dimServerName);
	FileContent dimFileContent(dimServerName);
	EndTransfer dimEndTransfer(dimServerName);

	DimServer::start(dimServerName.c_str());

	while(1){
		mainLoop();
	}
}

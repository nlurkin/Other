/*
 * NA62DimServerx.h
 *
 *  Created on: 30 Jan 2014
 *      Author: Nicolas Lurkin
 */
#ifndef NA62DIMSERVERX_H_
#define NA62DIMSERVERX_H_

#include <string>
#include "NA62DimCommands.h"
#include "dis.hxx"
#include <sstream>

/**
 * Pure virtual base class NA62DimServer.
 *
 * Implements the base functionalities specified in the note NA62-14-02.
 * Provides some convenience methods:
 * 		- print/println serie: output message both in cout and in the dimServerName/Info service.
 * 		- centralizedLog: output message in the dimServerName/Logging service with the correct standard format.
 * 		- setState/getState, waitConfigurationFile, moveToExpectedState: manage state transitions.
 *
 * Several methods are pure virtual and have to be implemented in the derived class:<br>
 * 		- mainLoop: main loop while waiting for dim actions
 * 		- generateConfig: generate a stream containing the current configuration. This string will be sent to the
 * 			RunControl to be stored in the condition database.
 * 		- applyConfiguration: apply the configuration that has been decoded from the configuration file
 */
class NA62DimServer: public DimServer
{
public:
	NA62DimServer(std::string name, int sourceID);
	virtual ~NA62DimServer();

	void initCommands(Command *cmdCommand=NULL, FileContent *fileContentCommand=NULL, RequestConfig *requestConfigCommand=NULL);
	void start();

	//Printing to Info
	void print(const char *s);
	void print(std::string s);
	void print(int s);
	void println(const char *s);
	void println(std::string s);
	void println(int s);

	//Logging
	void centralizedLog(int severity, std::string text, int priority, int errCode=0);

	//State management
	int getState() const;
	void setState(int state);

	void waitConfigurationFile(int expectedState);
	void moveToExpectedState();

	//virtual methods to implement
	/**
	 * MainLoop executed while waiting for dim actions.
	 *
	 * Does nothing by default.
	 */
	virtual void mainLoop(){};
	/**
	 * Generate the stream containing the current configuration of the device,
	 * to be published to dimServerName/Config and stored to condition database.
	 *
	 * To be implemented in the derived class.
	 * @param ss stringstream to fill
	 */
	virtual void generateConfig(std::stringstream& ss) = 0;
	/**
	 * Apply the configuration decoded from the configuration file.
	 *
	 * The decoded parameters should be found in the fConfigStruct.
	 * @return true if applying the configuration is successful, else false.
	 */
	virtual bool applyConfiguration() = 0;

	//Mandatory run infos
	int getRunNumber() const;
	void setRunNumber(int runNumber);

	/**
	 *
	 * @return Current Run Type
	 */
	std::string getRunType() const {return fRunType;};
	/**
	 * Set the current Run Type
	 * @param runType
	 */
	void setRuntype(std::string runType) {fRunType = runType;};

	/**
	 *
	 * @return Name of the dim server
	 */
	std::string getDimServerName() const {return fDimServerName;};

	/**
	 *
	 * @return Pointer to the fConfigStruct
	 */
	void* getConfigStruct() { return fConfigStruct;};

	//Config publishing
	void publishConfig();
private:
	/**
	 * Default constructor. Not implemented.
	 */
	NA62DimServer();
	/**
	 * Copy constructor. Not implemented.
	 */
	NA62DimServer(NA62DimServer&);
	/**
	 * Copy assignment operator. Not implemented.
	 */
	NA62DimServer& operator=(NA62DimServer&);

	void setNextState(int nextState);

	std::string fDimServerName;  /*!< Name of the dim server */

	//States: 0=IDLE, 1=INITIALIZED, 2=READY
	// <0 : ERROR
	int fState;		/*!< Current state of the device */
	int fNextState; /*!< Next expected state of the device after receiving and successfully applying the configuration file */
	char *fInfo;	/*!< Buffer for the dimServerName/Info service */
	char *fLogging;	/*!< Buffer for the dimServerName/Logging service */
	char *fConfig;	/*!< Buffer for the dimServerName/Config service */

	int fInfoIndex;	/*!< Current index in the fInfo buffer */

	DimService *fDimState;	/*!< Pointer to dimServerName/State service */
	DimService *fDimInfo;	/*!< Pointer to dimServerName/Info service */
	DimService *fDimLogging;/*!< Pointer to dimServerName/Logging service */
	DimService *fDimConfig;	/*!< Pointer to dimServerName/Config service */
	DimService *fDimVersion;	/*!< Pointer to dimServerName/NA62_VERSION service */

	Command *fDimCommand;				/*!< Pointer to the implementation of the dimServerName/Command dim command */
	FileContent *fDimFileContent;		/*!< Pointer to the implementation of the dimServerName/FileContent dim command */
	RequestConfig *fDimRequestConfig;	/*!< Pointer to the implementation of the dimServerName/RequestConfig dim command */

	int fRunNumber;			/*!< Current run number */
	std::string fRunType;	/*!< Current run type */

	int fSourceID;			/*!< Source id of this device */

	bool fIsStarted;			/*!< Has the server already been started */
protected:
	void* fConfigStruct;	/*!< Pointer to a structure containing the configuration parameters. Should be filled in FileContent derived class */
};


#endif /* NA62DIMSERVERX_H_ */

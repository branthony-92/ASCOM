#ifndef CAMSERVER_H
#define CAMSERVER_H

#include "RESTServer.h"
#include "CameraV1.h"

namespace CamServer {

	const std::string c_workingConfigPath = "Config/Config.json";

class CameraServer : public Server
{
public:

	struct ServerConfigurationData
	{
		std::string    configFilePath;
		std::string    IPv4Address;
		unsigned short port;
		unsigned int   serverThreadCount;
		bool           useHTTPS;
		std::string    certificateFilePath;
		std::string    privKeyFilePath;
		std::string    diffieHellmanParamFilePath;

	};

	CameraServer()
		: Server()
		, m_configData()
		, m_lastError()
	{}
	virtual ~CameraServer() {}

	bool start(ServerConfigurationData& info);
	bool stop();

	void saveConfigToFile(ServerConfigurationData& config);

private:
	SSLCtxInitHandler m_fHandler;

	DEVICEPROP(ServerConfigurationData, ConfigData, configData)
	DEVICEPROP(std::string,             LastError,   lastError)
};
typedef std::shared_ptr<CameraServer> CamServerPtr;
}
using ServerConfig = CamServer::CameraServer::ServerConfigurationData;

#endif // !CAMSERVER_H
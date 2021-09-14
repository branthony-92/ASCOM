#ifndef CAMSERVER_H
#define CAMSERVER_H

#include "RESTServer.h"
#include "CameraV1.h"

namespace CamServer {

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

	CameraServer(Camera::CamPtr pCam)
		: Server(pCam)
		, m_pCamera(pCam)
		, m_configData()
		, m_lastError()
	{}
	virtual ~CameraServer() {}

	bool start(ServerConfigurationData& info);
	bool stop();

private:
	SSLCtxInitHandler m_fHandler;

	DEVICEPROP(Camera::CamPtr, Camera, pCamera)
	DEVICEPROP(ServerConfigurationData, ConfigData, configData)
	DEVICEPROP(std::string, LastError, lastError)
};
typedef std::shared_ptr<CameraServer> CamServerPtr;

}

#endif // !CAMSERVER_H
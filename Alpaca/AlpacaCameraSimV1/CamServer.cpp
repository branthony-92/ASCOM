#include "stdafx.h"
#include "CamServer.h"
#include <fstream>
#include <iomanip>

using namespace DevServer;

bool DeviceServer::start(ServerConfigurationData& info)
{
	if (m_serverContexts.empty())
	{
		m_lastError = "No Devices Registered";
		return false;
	}

	m_pInfo->setServerName("Alpaca Camera Simulator Server");
	m_pInfo->setAPIVer(1.0);
	m_pInfo->setIdleTimeout(UINT32_MAX); // 5 min timeout
	m_pInfo->getURI()->setAddress(info.IPv4Address);
	m_pInfo->getURI()->setPort(info.port);

	auto& endpointList = m_pInfo->getEndpointNames();
	endpointList.clear();
	for (auto& pDevice : m_serverContexts)
	{
		endpointList.push_back(pDevice->getHandlerInfo());
	}

	auto certificateFilePath = info.certificateFilePath;
	auto privKeyPath         = info.privKeyFilePath;
	auto dhParamPath         = info.diffieHellmanParamFilePath;

	auto ssl_callback = [certificateFilePath, privKeyPath, dhParamPath](boost::asio::ssl::context& ctx) -> bool
	{
		try {
			printf("SSL Context Callback entered\n");
			ctx.set_options(
				ssl::context::default_workarounds |
				ssl::context::no_sslv2 |
				ssl::context::no_tlsv1 |
				ssl::context::no_tlsv1_1 |
				ssl::context::single_dh_use
			);

			ctx.set_password_callback(
				[](std::size_t /*max_length*/, ssl::context::password_purpose /*purpose*/)
			{ return "test"; }
			);
			beast::error_code ec;

			ctx.use_certificate_chain_file(certificateFilePath);
			ctx.use_private_key_file(privKeyPath, ssl::context::pem);
			ctx.use_tmp_dh_file(dhParamPath);

			return true;
		}
		catch (std::exception const& e) {
			std::cout << "ERROR: " << e.what() << "\n";
			return false;
		}
	};

	SessionPtr pSessionPrototype = nullptr;

	bool serverStarted = false;

	std::string reasonStr = "";

	if (info.useHTTPS)
	{
		pSessionPrototype = std::make_shared<GenericSessionHTTPS>(m_serverContexts);
		reasonStr = "Failed to start HTTPS server";
	}
	else
	{
		pSessionPrototype = std::make_shared<GenericSessionHTTP>(m_serverContexts);
		reasonStr = "Failed to start HTTP server";
	}
	serverStarted = startServer(info.IPv4Address, info.port, pSessionPrototype, ssl_callback);

	if (!serverStarted)
	{
		// nothin to do with no working server
		m_lastServerError = "Could not start server: " + reasonStr;
		return false;
	}
	m_pInfo->setState(ServerInfoBody::ServerStatus::Listening);
	saveConfigToFile(info);
	return true;
}

bool DeviceServer::stop()
{
	reset();
	return true;
}

void DevServer::DeviceServer::saveConfigToFile(ServerConfig& config)
{
	// try to load the file if one exists
	JSON cfgJSON = jsonUtils::loadFile(c_workingConfigPath);
	JSON serverInfo = jsonUtils::extractValue<JSON>(cfgJSON, "Server_Config", JSON::object());

	// update the server info section
	serverInfo["Port"] = config.port;
	serverInfo["IPv4"] = config.IPv4Address;
	serverInfo["Certificate_Path"] = config.certificateFilePath;
	serverInfo["Priv_Key_Path"] = config.privKeyFilePath;
	serverInfo["DH_Params_Path"] = config.diffieHellmanParamFilePath;
	serverInfo["Enable_HTTPS"] = config.useHTTPS;
	serverInfo["Server_Thread_Count"] = config.serverThreadCount;

	cfgJSON["Server_Config"] = serverInfo;

	std::ofstream configFile;
	configFile.open(c_workingConfigPath, std::fstream::trunc);
	if (!configFile.is_open()) return;

	configFile << std::setw(4) << cfgJSON << std::endl;
	setConfigData(config);
}

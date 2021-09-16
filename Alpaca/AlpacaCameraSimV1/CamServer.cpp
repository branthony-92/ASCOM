#include "stdafx.h"
#include "CamServer.h"

using namespace CamServer;

bool CameraServer::start(ServerConfigurationData& info)
{
	if (m_serverContexts.empty())
	{
		m_lastError = "No Devices Registered";
		return false;
	}

	m_pInfo->setServerName("Alpaca Camera Simulator Server");
	m_pInfo->setAPIVer(1.0);
	m_pInfo->setIdleTimeout(300u); // 5 min timeout

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
	setConfigData(info);
	return true;
}

bool CameraServer::stop()
{
	reset();
	return true;
}

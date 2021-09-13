#include "stdafx.h"
#include <QtCore/QCoreApplication>
#include "RESTServer.h"
#include "CameraV1.h"
#include "CameraStateMachine.h"

const std::string c_configFilePath = "C:\\Users\\xs_br\\source\\repos\\branthony-92\\ASCOM\\Alpaca\\AlpacaCameraSimV1\\Config.json";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	auto pCam = std::make_shared<CameraV1>(0);
	auto pServer = std::make_shared<Server>(pCam, 8);

	// init the state machine for the camera
	auto pCamStateMachine = std::make_shared<CameraStateMachine>(pCam);
	
	auto pInfo = pServer->getServerInfo();
	pInfo->setServerName("Alpaca Camera Simulator Server");
	pInfo->setAPIVer(1.0);
	pInfo->setIdleTimeout(300u); // 5 min timeout
	pCam->setServerInfo(pInfo);

	nlohmann::json config = jsonUtils::loadFile(c_configFilePath);
	nlohmann::json serverConfig = jsonUtils::extractValue<nlohmann::json>(config, "Server_Config", nlohmann::json::object());
	bool useHTTPS = jsonUtils::extractValue<bool>(serverConfig, "Enable_HTTPS", false);

	auto ssl_callback = [](boost::asio::ssl::context& ctx) {
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

			nlohmann::json config = jsonUtils::loadFile(c_configFilePath);
			if (config.empty())
			{
				throw std::runtime_error("Configuration file not found");
			}

			nlohmann::json serverConfig = jsonUtils::extractValue<nlohmann::json>(config, "Server_Config", nlohmann::json::object());
			if (serverConfig.empty())
			{
				throw std::runtime_error("Server configuration not found");
			}

			std::string certPath = jsonUtils::extractValue<std::string>(serverConfig, "Certificate_Path", "");
			if (certPath.empty()) throw std::runtime_error("Path to certificate not found");

			std::string keyPath = jsonUtils::extractValue<std::string>(serverConfig, "Priv_Key_Path", "");
			if (keyPath.empty()) throw std::runtime_error("Path to private key not found");

			std::string dhParamPath = jsonUtils::extractValue<std::string>(serverConfig, "DH_Params_Path", "");
			if (dhParamPath.empty()) throw std::runtime_error("Path to Diffie-Hellman parameters not found");

			ctx.use_certificate_chain_file(certPath);
			ctx.use_private_key_file(keyPath, ssl::context::pem);
			ctx.use_tmp_dh_file(dhParamPath);
		}
		catch (std::exception const& e) {
			std::cout << "ERROR: " << e.what() << "\n";
		}
	};

	SessionPtr pSessionPrototype = nullptr;
	if (useHTTPS) 
	{ 
		pSessionPrototype = std::make_shared<GenericSessionHTTPS>(pCam);
	}
	else 
	{ 
		pSessionPrototype = std::make_shared<GenericSessionHTTP>(pCam); 
	}

	pServer->startServer("192.168.2.16", 8080, pSessionPrototype, ssl_callback);
	while (true);

    return a.exec();
}

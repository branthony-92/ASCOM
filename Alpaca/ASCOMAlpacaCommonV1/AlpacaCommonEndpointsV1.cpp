#include "stdafx.h"
#include "AlpacaCommonEndpointsV1.h"
#include "AlpacaResponseBodiesV1.h"
#include "AlpacaDeviceV1.h"
#include "Helpers.h"
#include "ErrorCodes.h"

using namespace Alpaca;
using namespace Alpaca::Common;


const std::map<CommonEndpointID, std::string> c_epTargetReflections = {
	{ CommonEndpointID::epAction,           "/action" },
	{ CommonEndpointID::epCmdBlind,			"/commandblind" },
	{ CommonEndpointID::epCmdBool,			"/commandbool" },
	{ CommonEndpointID::epCmdString,		"/commandstring" },
	{ CommonEndpointID::epConnected,		"/connected" },
	{ CommonEndpointID::epDescription,		"/description" },
	{ CommonEndpointID::epDriverInfo,		"/driverinfo" },
	{ CommonEndpointID::epDriverVer,		"/driverversion" },
	{ CommonEndpointID::epInterfaceVer,		"/interfaceversion" },
	{ CommonEndpointID::epName,				"/name" },
	{ CommonEndpointID::epSupportedActions,	"/supportedactions" }
};

std::shared_ptr<HTTPRequestHandler> Alpaca::Common::createHandler(const CommonEndpointID id, std::string epPrefix)
{
	std::shared_ptr<HTTPRequestHandler> pHandler = nullptr;

	auto method = c_epTargetReflections.find(id);

	if (method == c_epTargetReflections.end()) return nullptr;

	std::string fullEndpointString = epPrefix + method->second;

	switch (id)
	{
		case CommonEndpointID::epAction:
			pHandler = std::make_shared<Endpoint_Action>(fullEndpointString);
			break;
		case CommonEndpointID::epCmdBlind:
			pHandler = std::make_shared<Endpoint_CommandBlind>(fullEndpointString);
			break;
		case CommonEndpointID::epCmdBool:
			pHandler = std::make_shared<Endpoint_CommandBool>(fullEndpointString);
			break;
		case CommonEndpointID::epCmdString:
			pHandler = std::make_shared<Endpoint_CommandString>(fullEndpointString);
			break;
		case CommonEndpointID::epConnected:
			pHandler = std::make_shared<Endpoint_Connected>(fullEndpointString);
			break;
		case CommonEndpointID::epDescription:
			pHandler = std::make_shared<Endpoint_Description>(fullEndpointString);
			break;
		case CommonEndpointID::epDriverInfo:
			pHandler = std::make_shared<Endpoint_DriverInfo>(fullEndpointString);
			break;
		case CommonEndpointID::epDriverVer:
			pHandler = std::make_shared<Endpoint_DriverVersion>(fullEndpointString);
			break;
		case CommonEndpointID::epInterfaceVer:
			pHandler = std::make_shared<Endpoint_InterfaceVersion>(fullEndpointString);
			break;
		case CommonEndpointID::epName:
			pHandler = std::make_shared<Endpoint_Name>(fullEndpointString);
			break;
		case CommonEndpointID::epSupportedActions:
			pHandler = std::make_shared<Endpoint_SupportedActions>(fullEndpointString);
			break;
	default:
		break;
	}
	return pHandler;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_Action::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{
		throw std::exception("No Device Object");
	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	// parse the body
	
	if (body.empty())
	{
		pResponseBody->setErrorMessage("No Request Body Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.contains(Common::c_paramKeyClientID))
	{
		auto clientID = body.extract(Common::c_paramKeyClientID, 0);
	}

	if (body.contains(Common::c_paramKeyClientTransactionID))
	{
		auto transactionID = body.extract(Common::c_paramKeyClientTransactionID, 0);
		if (transactionID > 0)
		{
			pResponseBody->setClientTransactionID(++transactionID);
		}
	}


	if (!body.contains("Action"))
	{
		pResponseBody->setErrorMessage("No Action Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}
	if (!body.contains("Parameters"))
	{
		pResponseBody->setErrorMessage("No Parameters Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto action = body.extract("Action");

	auto supportedActions = pDevice->getSupportedActions();
	bool supported = false;

	for (auto a : supportedActions)
	{
		if (action == a)
		{
			supported = true;
			break;
		}
	}

	if (!supported)
	{
		pResponseBody->setErrorMessage("Action is not supported");
		pResponseBody->setErrorNumber(ErrorCodes::c_notImplemented);
		return pResponseBody;
	}

	auto params = body.extract("Parameters");

	auto ret = pDevice->action(action, params);

	pResponseBody->setValue(ret);

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_CommandBlind::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{
		throw std::exception("No Device Object");
	}
	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	// parse the body


	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorMessage("No Request Body Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	if (body.contains(Common::c_paramKeyClientID))
	{
		auto clientID = body.extract(Common::c_paramKeyClientID, 0);
	}

	if (body.contains(Common::c_paramKeyClientTransactionID))
	{
		auto transactionID = body.extract(Common::c_paramKeyClientTransactionID, 0);
		if (transactionID > 0)
		{
			pResponseBody->setClientTransactionID(++transactionID);
		}
	}

	if (!body.contains("Command"))
	{
		pResponseBody->setErrorMessage("Command Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}
	if (!body.contains("Raw"))
	{
		pResponseBody->setErrorMessage("Raw Specification Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto cmd    = body.extract("Command");
	auto raw	= body.extract("Raw");

	pDevice->commandBlind(cmd, raw);

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_CommandBool::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	// parse the body
	

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorMessage("No Request Body Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	if (body.contains(Common::c_paramKeyClientID))
	{
		auto clientID = body.extract(Common::c_paramKeyClientID, 0);
	}

	if (body.contains(Common::c_paramKeyClientTransactionID))
	{
		auto transactionID = body.extract(Common::c_paramKeyClientTransactionID, 0);
		if (transactionID > 0)
		{
			pResponseBody->setClientTransactionID(++transactionID);
		}
	}

	if (!body.contains("Command"))
	{
		pResponseBody->setErrorMessage("Command Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}
	if (!body.contains("Raw"))
	{
		pResponseBody->setErrorMessage("Raw Specification Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto cmd = body.extract("Command");
	auto raw = body.extract("Raw");


	auto ret = pDevice->commandBool(cmd, raw);

	pResponseBody->setValue(ret);

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_CommandString::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	// parse the body
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorMessage("No Request Body Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	if (body.contains(Common::c_paramKeyClientID))
	{
		auto clientID = body.extract(Common::c_paramKeyClientID, 0);
	}

	if (body.contains(Common::c_paramKeyClientTransactionID))
	{
		auto transactionID = body.extract(Common::c_paramKeyClientTransactionID, 0);
		if (transactionID > 0)
		{
			pResponseBody->setClientTransactionID(++transactionID);
		}
	}

	if (!body.contains("Command"))
	{
		pResponseBody->setErrorMessage("Command Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}
	if (!body.contains("Raw"))
	{
		pResponseBody->setErrorMessage("Raw Specification Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto cmd = body.extract("Command");
	auto raw = body.extract("Raw");


	auto ret = pDevice->commandString(cmd, raw);

	pResponseBody->setValue(ret);

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_Connected::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getConnected());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_Connected::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	// parse the body
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorMessage("No Request Body Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	if (body.contains(Common::c_paramKeyClientID))
	{
		auto clientID = body.extract(Common::c_paramKeyClientID, 0);
	}

	if (body.contains(Common::c_paramKeyClientTransactionID))
	{
		auto transactionID = body.extract(Common::c_paramKeyClientTransactionID, 0);
		if (transactionID > 0)
		{
			pResponseBody->setClientTransactionID(++transactionID);
		}
	}

	if (!body.contains("Connected"))
	{
		pResponseBody->setErrorMessage("Connection State Not Provided");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto newConnectedStatus = body.extract("Connected", false);

	if (pDevice->getConnected() && !newConnectedStatus)
	{
		pDevice->disconnect();
	}
	else if (!pDevice->getConnected() && newConnectedStatus)
	{
		pDevice->connect();
	}

	pDevice->setConnected(newConnectedStatus);

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_Description::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getDescription());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_DriverInfo::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);
	
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getDriverInfo());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_DriverVersion::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);
	
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getDriverVersion());
	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_InterfaceVersion::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);
	
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getInterfaceVersion());
	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_Name::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}

	auto type = queries.extract(c_paramKeyDeviceType);
	
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getName());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Common::Endpoint_SupportedActions::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pDevice = std::dynamic_pointer_cast<AlpacaDeviceV1>(pCtx);
	if (!pDevice)
	{

	}
	auto pResponseBody = std::make_shared<Bodies::StringArrayResponse>();

	if (!(queries.contains(c_paramKeyDeviceType) && queries.contains("c_paramKeyDeviceNumber")))
	{
		// missing queries
	}
	
	auto type      = queries.extract(c_paramKeyDeviceType);
	
	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(c_paramKeyClientID, -1);
	auto transactionID = queries.extract(c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pDevice->getSupportedActions());
	return pResponseBody;
}

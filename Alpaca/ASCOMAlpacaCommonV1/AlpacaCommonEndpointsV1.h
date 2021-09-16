#ifndef ALPACACOMMENENDPOINTSV1_H
#define ALPACACOMMENENDPOINTSV1_H

#include "RESTEndpoint.h"

namespace Alpaca {
namespace Common {

const std::string c_paramKeyDeviceType          = "device_type";
const std::string c_paramKeyDeviceNumber        = "device_number";
const std::string c_paramKeyClientID            = "ClientID";
const std::string c_paramKeyClientTransactionID = "ClientTransactionID";

enum class CommonEndpointID
{
	epFirst = 0,
	epAction = epFirst,
	epCmdBlind,
	epCmdBool, 
	epCmdString,
	epConnected,
	epDescription,
	epDriverInfo,
	epDriverVer,
	epInterfaceVer,
	epName,
	epSupportedActions,

	epLast,
};

std::shared_ptr<HTTPRequestHandler> createHandler(const CommonEndpointID, std::string);

class Endpoint_Action : public HTTPRequestHandler
{
public:
	Endpoint_Action(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CommandBlind : public HTTPRequestHandler
{
public:
	Endpoint_CommandBlind(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CommandBool : public HTTPRequestHandler
{
public:
	Endpoint_CommandBool(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CommandString : public HTTPRequestHandler
{
public:
	Endpoint_CommandString(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Connected : public HTTPRequestHandler
{
public:
	Endpoint_Connected(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};


class Endpoint_Description : public HTTPRequestHandler
{
public:
	Endpoint_Description(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_DriverInfo : public HTTPRequestHandler
{
public:
	Endpoint_DriverInfo(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_DriverVersion: public HTTPRequestHandler
{
public:
	Endpoint_DriverVersion(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_InterfaceVersion : public HTTPRequestHandler
{
public:
	Endpoint_InterfaceVersion(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Name : public HTTPRequestHandler
{
public:
	Endpoint_Name(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_SupportedActions : public HTTPRequestHandler
{
public:
	Endpoint_SupportedActions(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::RequestType::APIRequest)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

} // end Common
} // end Alpaca
#endif // !ALPACACOMMENENDPOINTSV1_H
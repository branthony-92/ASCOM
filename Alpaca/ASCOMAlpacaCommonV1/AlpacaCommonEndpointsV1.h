#ifndef ALPACACOMMENENDPOINTSV1_H
#define ALPACACOMMENENDPOINTSV1_H

#include "RESTEndpoint.h"

namespace Alpaca {
	namespace Common {
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
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_CommandBlind : public HTTPRequestHandler
{
public:
	Endpoint_CommandBlind(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_CommandBool : public HTTPRequestHandler
{
public:
	Endpoint_CommandBool(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_CommandString : public HTTPRequestHandler
{
public:
	Endpoint_CommandString(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_Connected : public HTTPRequestHandler
{
public:
	Endpoint_Connected(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};


class Endpoint_Description : public HTTPRequestHandler
{
public:
	Endpoint_Description(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_DriverInfo : public HTTPRequestHandler
{
public:
	Endpoint_DriverInfo(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_DriverVersion: public HTTPRequestHandler
{
public:
	Endpoint_DriverVersion(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_InterfaceVersion : public HTTPRequestHandler
{
public:
	Endpoint_InterfaceVersion(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_Name : public HTTPRequestHandler
{
public:
	Endpoint_Name(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};

class Endpoint_SupportedActions : public HTTPRequestHandler
{
public:
	Endpoint_SupportedActions(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx);
};
}
}
#endif // !ALPACACOMMENENDPOINTSV1_H
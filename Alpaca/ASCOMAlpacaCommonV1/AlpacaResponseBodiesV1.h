#ifndef AlpacaResponseBodiesV1_H
#define AlpacaResponseBodiesV1_H

#include "RESTModels.h"
#include "AlpacaUtils.h"

namespace Alpaca {
namespace Bodies {
class MethodResponse : public JSONInfoBody
{
		DEVICEPROP(unsigned int, ClientTransactionID, clientTransactionID)
		DEVICEPROP(unsigned int, ServerTransactionID, serverTransactionID)
		DEVICEPROP(int, ErrorNumber, errorNumber)
		DEVICEPROP(std::string, ErrorMessage, errorMessage)

public:
	MethodResponse()
		: JSONInfoBody("MethodResponse")
		, m_clientTransactionID(0)
		, m_serverTransactionID(0)
		, m_errorNumber(0)
		, m_errorMessage()
	{}
	virtual ~MethodResponse() {}

	virtual JSON  toJSON() const override
	{
		JSON info = JSON::object();
		info["ClientTransactionID"] = m_clientTransactionID;
		info["ServerTransactionID"] = m_serverTransactionID;
		info["ErrorNumber"]         = m_errorNumber;
		info["ErrorMessage"]        = m_errorMessage;

		return info;
	}
	virtual void  loadJSON(JSON info) override
	{
		m_clientTransactionID = jsonUtils::extractValue<unsigned int>(info, "ClientTransactionID", 0);
		m_serverTransactionID = jsonUtils::extractValue<unsigned int>(info, "ServerTransactionID", 0);
		m_errorNumber         = jsonUtils::extractValue<int>(info, "ErrorNumber", -1);
		m_errorMessage        = jsonUtils::extractValue<std::string>(info, "ErrorMessage", "");
	}
};

template<typename T>
class ValueResponse : public JSONInfoBody
{
	DEVICEPROP(T,            Value,               value)
	DEVICEPROP(unsigned int, ClientTransactionID, clientTransactionID)
	DEVICEPROP(unsigned int, ServerTransactionID, serverTransactionID)
	DEVICEPROP(int,          ErrorNumber,         errorNumber)
	DEVICEPROP(std::string,  ErrorMessage,        errorMessage)

public:
	ValueResponse() 
		: JSONInfoBody("ValueResponse")
		, m_value()
		, m_clientTransactionID(0)
		, m_serverTransactionID(0)
		, m_errorNumber(0)
		, m_errorMessage()
	{}
	virtual ~ValueResponse() {}

	virtual JSON toJSON() const override
	{
		JSON info = JSON::object();

		info["Value"]				= m_value;
		info["ClientTransactionID"] = m_clientTransactionID;
		info["ServerTransactionID"] = m_serverTransactionID;
		info["ErrorNumber"]			= m_errorNumber;
		info["ErrorMessage"]		= m_errorMessage;

		return info;
	}
	virtual void  loadJSON(JSON info) override
	{
		m_value					= jsonUtils::extractValue<T>(info, "Value", T());
		m_clientTransactionID	= jsonUtils::extractValue<unsigned int>(info, "ClientTransactionID", 0);
		m_serverTransactionID	= jsonUtils::extractValue<unsigned int>(info, "ServerTransactionID", 0);
		m_errorNumber			= jsonUtils::extractValue<int>(info, "ErrorNumber", -1);
		m_errorMessage			= jsonUtils::extractValue<std::string>(info, "ErrorMessage", "");
	}
};

// single values
typedef ValueResponse<std::string> StringResponse;
typedef ValueResponse<int>         IntResponse;
typedef ValueResponse<double>      DoubleResponse;
typedef ValueResponse<bool>        BoolResponse;

// array values
typedef ValueResponse<std::vector<std::string>> StringArrayResponse;
typedef ValueResponse<std::vector<int>>			IntArrayResponse;

}
}
#endif // !AlpacaResponseBodiesV1_H
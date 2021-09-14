#ifndef ALPACACAMENDPOINTSV1_H
#define ALPACACAMENDPOINTSV1_H

#include "RESTEndpoint.h"
#include "AlpacaCommonEndpointsV1.h"

namespace Alpaca {
namespace Camera {

const unsigned int nextEndpointID = static_cast<unsigned int>(Alpaca::Common::CommonEndpointID::epLast);

enum class CamEndpointID
{
	epFirst = nextEndpointID,

	epBayerOffsetX = epFirst,
	epBayerOffsetY,
	epBinX,
	epBinY,
	epCameraState,
	epCameraXSize,
	epCameraYSize,
	epCanAbortExposure,
	epCanAsymmetricBin,
	epCanFastReadout,
	epCanGetCoolerPower,
	epCanPulseGuide,
	epCanSetCCDTemp,
	epCanStopExposure,
	epCCDTemp,
	epCoolerOn,
	epCoolerPower,
	epElectronsPerAdu,
	epExposureMin,
	epExposureMax,
	epExposureResolution,
	epFastReadout,
	epFullWellCapacity,
	epGain,
	epGainMax,
	epGainMin,
	epGains,
	epHasShutter,
	epHeatsinkTemp,
	epImageArray,
	epImageArrayVariant,
	epImageReady,
	epIsPulseGuiding,
	epLastExposureDuration,
	epLastExposureStartTime,
	epMaxADU,
	epMaxBinX,
	epMaxBinY,
	epNumX,
	epNumY,
	epOffset,
	epOffsetMin,
	epOffsetMax,
	epOffsets,
	epPercentComplete,
	epPixelSizeX,
	epPixelSizeY,
	epReadoutMode,
	epReadoutModes,
	epSensorName,
	epSensorType,
	epCoolerSetpoint,
	epStartX,
	epStartY,
	epSubExposureDuration,
	epAbortExposure,
	epPulseGuide,
	epStartExposure,
	epStopExposure,
	epLast
};

std::shared_ptr<HTTPRequestHandler> createHandler(const CamEndpointID, std::string);

// properties
class Endpoint_BayerOffsetX : public HTTPRequestHandler
{
public:
	Endpoint_BayerOffsetX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_BayerOffsetY : public HTTPRequestHandler
{
public:
	Endpoint_BayerOffsetY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_BinX : public HTTPRequestHandler
{
public:
	Endpoint_BinX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_BinY : public HTTPRequestHandler
{
public:
	Endpoint_BinY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CameraState : public HTTPRequestHandler
{
public:
	Endpoint_CameraState(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CameraXSize : public HTTPRequestHandler
{
public:
	Endpoint_CameraXSize(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CameraYSize : public HTTPRequestHandler
{
public:
	Endpoint_CameraYSize(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanAbortExposure : public HTTPRequestHandler
{
public:
	Endpoint_CanAbortExposure(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanAsymmetricBin : public HTTPRequestHandler
{
public:
	Endpoint_CanAsymmetricBin(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanFastReadout : public HTTPRequestHandler
{
public:
	Endpoint_CanFastReadout(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanGetCoolerPower : public HTTPRequestHandler
{
public:
	Endpoint_CanGetCoolerPower(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanPulseGuide : public HTTPRequestHandler
{
public:
	Endpoint_CanPulseGuide(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanSetCCDTemp : public HTTPRequestHandler
{
public:
	Endpoint_CanSetCCDTemp(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CanStopExposure : public HTTPRequestHandler
{
public:
	Endpoint_CanStopExposure(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CCDTemp : public HTTPRequestHandler
{
public:
	Endpoint_CCDTemp(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CoolerOn: public HTTPRequestHandler
{
public:
	Endpoint_CoolerOn(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CoolerPower : public HTTPRequestHandler
{
public:
	Endpoint_CoolerPower(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ElectronsPerADU : public HTTPRequestHandler
{
public:
	Endpoint_ElectronsPerADU(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ExposureMax : public HTTPRequestHandler
{
public:
	Endpoint_ExposureMax(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ExposureMin : public HTTPRequestHandler
{
public:
	Endpoint_ExposureMin(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ExposureResolution : public HTTPRequestHandler
{
public:
	Endpoint_ExposureResolution(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_FastReadout : public HTTPRequestHandler
{
public:
	Endpoint_FastReadout(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_FullWellCapacity : public HTTPRequestHandler
{
public:
	Endpoint_FullWellCapacity(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Gain : public HTTPRequestHandler
{
public:
	Endpoint_Gain(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_GainMax : public HTTPRequestHandler
{
public:
	Endpoint_GainMax(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_GainMin : public HTTPRequestHandler
{
public:
	Endpoint_GainMin(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Gains : public HTTPRequestHandler
{
public:
	Endpoint_Gains(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_HasShutter : public HTTPRequestHandler
{
public:
	Endpoint_HasShutter(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_HeatSinkTemp : public HTTPRequestHandler
{
public:
	Endpoint_HeatSinkTemp(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ImageArray : public HTTPRequestHandler
{
public:
	Endpoint_ImageArray(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ImageArrayVariant : public HTTPRequestHandler
{
public:
	Endpoint_ImageArrayVariant(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ImageReady : public HTTPRequestHandler
{
public:
	Endpoint_ImageReady(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_IsPulseGuiding : public HTTPRequestHandler
{
public:
	Endpoint_IsPulseGuiding(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_LastExposureDuration : public HTTPRequestHandler
{
public:
	Endpoint_LastExposureDuration(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_LastExposureStartTime : public HTTPRequestHandler
{
public:
	Endpoint_LastExposureStartTime(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_MaxADU : public HTTPRequestHandler
{
public:
	Endpoint_MaxADU(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_MaxBinX : public HTTPRequestHandler
{
public:
	Endpoint_MaxBinX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_MaxBinY : public HTTPRequestHandler
{
public:
	Endpoint_MaxBinY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_NumX : public HTTPRequestHandler
{
public:
	Endpoint_NumX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_NumY : public HTTPRequestHandler
{
public:
	Endpoint_NumY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Offset : public HTTPRequestHandler
{
public:
	Endpoint_Offset(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_OffsetMax : public HTTPRequestHandler
{
public:
	Endpoint_OffsetMax(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_OffsetMin : public HTTPRequestHandler
{
public:
	Endpoint_OffsetMin(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_Offsets : public HTTPRequestHandler
{
public:
	Endpoint_Offsets(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_PercentComplete : public HTTPRequestHandler
{
public:
	Endpoint_PercentComplete(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_PixelSizeX : public HTTPRequestHandler
{
public:
	Endpoint_PixelSizeX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_PixelSizeY : public HTTPRequestHandler
{
public:
	Endpoint_PixelSizeY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ReadoutMode : public HTTPRequestHandler
{
public:
	Endpoint_ReadoutMode(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_ReadoutModes : public HTTPRequestHandler
{
public:
	Endpoint_ReadoutModes(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_SensorName : public HTTPRequestHandler
{
public:
	Endpoint_SensorName(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_SensorType : public HTTPRequestHandler
{
public:
	Endpoint_SensorType(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_CoolerSetpoint : public HTTPRequestHandler
{
public:
	Endpoint_CoolerSetpoint(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_StartX : public HTTPRequestHandler
{
public:
	Endpoint_StartX(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_StartY : public HTTPRequestHandler
{
public:
	Endpoint_StartY(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_SubExposureDuration : public HTTPRequestHandler
{
public:
	Endpoint_SubExposureDuration(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

// methods
class Endpoint_AbortExposure : public HTTPRequestHandler
{
public:
	Endpoint_AbortExposure(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_PulseGuide : public HTTPRequestHandler
{
public:
	Endpoint_PulseGuide(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_StartExposure: public HTTPRequestHandler
{
public:
	Endpoint_StartExposure(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

class Endpoint_StopExposure : public HTTPRequestHandler
{
public:
	Endpoint_StopExposure(std::string epString)
		: HTTPRequestHandler(epString, HTTPRequestHandler::DataType::JSON)
	{}

	std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx) override;
};

} // End Camera
} // End Alpaca
#endif // !ALPACACAMENDPOINTSV1_H
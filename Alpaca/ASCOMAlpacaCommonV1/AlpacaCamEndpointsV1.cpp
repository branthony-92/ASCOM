#include "stdafx.h"
#include "AlpacaResponseBodiesV1.h"
#include "AlpacaCamBodiesV1.h"
#include "AlpacaCamEndpointsV1.h"
#include "IAlpacaCamV1.h"
#include "ErrorCodes.h"
#include "Helpers.h"

using namespace Alpaca;

const std::map<Camera::CamEndpointID, std::string> c_epTargetReflections = {
	{ Camera::CamEndpointID::epBayerOffsetX,                 "/bayeroffsetx"          },
	{ Camera::CamEndpointID::epBayerOffsetY,				 "/bayeroffsety"          },
	{ Camera::CamEndpointID::epBinX,						 "/binx"                  },
	{ Camera::CamEndpointID::epBinY,						 "/biny"                  },
	{ Camera::CamEndpointID::epCameraState,					 "/camerastate"           },
	{ Camera::CamEndpointID::epCameraXSize,					 "/cameraxsize"           },
	{ Camera::CamEndpointID::epCameraYSize,					 "/cameraysize"           },
	{ Camera::CamEndpointID::epCanAbortExposure,			 "/canabortexposure"      },
	{ Camera::CamEndpointID::epCanAsymmetricBin,			 "/canasymmetricbin"      },
	{ Camera::CamEndpointID::epCanFastReadout,				 "/canfastreadout"        },
	{ Camera::CamEndpointID::epCanGetCoolerPower,			 "/cangetcoolerpower"     },
	{ Camera::CamEndpointID::epCanPulseGuide,				 "/canpulseguide"         },
	{ Camera::CamEndpointID::epCanSetCCDTemp,				 "/cansetccdtemperature"  },
	{ Camera::CamEndpointID::epCanStopExposure,				 "/canstopexposure"       },
	{ Camera::CamEndpointID::epCCDTemp,						 "/ccdtemperature"        },
	{ Camera::CamEndpointID::epCoolerOn,					 "/cooleron"              },
	{ Camera::CamEndpointID::epCoolerPower,					 "/coolerpower"           },
	{ Camera::CamEndpointID::epElectronsPerAdu,				 "/electronsperadu"       },
	{ Camera::CamEndpointID::epExposureMin,					 "/exposuremin"           },
	{ Camera::CamEndpointID::epExposureMax,					 "/exposuremax"           },
	{ Camera::CamEndpointID::epExposureResolution,			 "/exposureresolution"    },
	{ Camera::CamEndpointID::epFastReadout,					 "/fastreadout"           },
	{ Camera::CamEndpointID::epFullWellCapacity,			 "/fullwellcapacity"      },
	{ Camera::CamEndpointID::epGain,						 "/gain"                  },
	{ Camera::CamEndpointID::epGainMax,						 "/gainmax"               },
	{ Camera::CamEndpointID::epGainMin,						 "/gainmin"               },
	{ Camera::CamEndpointID::epGains,						 "/gains"                 },
	{ Camera::CamEndpointID::epHasShutter,					 "/hasshutter"            },
	{ Camera::CamEndpointID::epHeatsinkTemp,				 "/heatsinktemperature"   },
	{ Camera::CamEndpointID::epImageArray,					 "/imagearray"            },
	{ Camera::CamEndpointID::epImageArrayVariant,			 "/imagearrayvariant"     },
	{ Camera::CamEndpointID::epImageReady,					 "/imageready"            },
	{ Camera::CamEndpointID::epIsPulseGuiding,				 "/ispulseguiding"        },
	{ Camera::CamEndpointID::epLastExposureDuration,		 "/lastexposureduration"  },
	{ Camera::CamEndpointID::epLastExposureStartTime,		 "/lastexposurestarttime" },
	{ Camera::CamEndpointID::epMaxADU,						 "/maxadu"				  },
	{ Camera::CamEndpointID::epMaxBinX,						 "/maxbinx"				  },
	{ Camera::CamEndpointID::epMaxBinY,						 "/maxbiny"				  },
	{ Camera::CamEndpointID::epNumX,						 "/numx"				  },
	{ Camera::CamEndpointID::epNumY,						 "/numy"				  },
	{ Camera::CamEndpointID::epOffset,						 "/offset"				  },
	{ Camera::CamEndpointID::epOffsetMin,					 "/offsetmin"			  },
	{ Camera::CamEndpointID::epOffsetMax,					 "/offsetmax"			  },
	{ Camera::CamEndpointID::epOffsets,						 "/offsets"               },
	{ Camera::CamEndpointID::epPercentComplete,				 "/percentcomplete"       },
	{ Camera::CamEndpointID::epPixelSizeX,					 "/pixelsizex"            },
	{ Camera::CamEndpointID::epPixelSizeY,					 "/pixelsizey"            },
	{ Camera::CamEndpointID::epReadoutMode,					 "/readoutmode"           },
	{ Camera::CamEndpointID::epReadoutModes,				 "/readoutmodes"          },
	{ Camera::CamEndpointID::epSensorName,					 "/sensorname"            },
	{ Camera::CamEndpointID::epSensorType,					 "/sensortype"            },
	{ Camera::CamEndpointID::epCoolerSetpoint,				 "/setccdtemperature"     },
	{ Camera::CamEndpointID::epStartX,						 "/startx"                },
	{ Camera::CamEndpointID::epStartY,						 "/starty"                },
	{ Camera::CamEndpointID::epSubExposureDuration,			 "/subexposureduration"   },
	{ Camera::CamEndpointID::epAbortExposure,                "/abortexposure"         },
	{ Camera::CamEndpointID::epPulseGuide,					 "/pulseguide"            },
	{ Camera::CamEndpointID::epStartExposure,				 "/startexposure"		  },
	{ Camera::CamEndpointID::epStopExposure,				 "/stopexposure"		  }
};

std::shared_ptr<HTTPRequestHandler> Alpaca::Camera::createHandler(const Camera::CamEndpointID id, std::string epPrefix)
{
	std::shared_ptr<HTTPRequestHandler> pHadler = nullptr;
	auto method = c_epTargetReflections.find(id);

	if (method == c_epTargetReflections.end()) return nullptr;

	std::string fullEndpointString = epPrefix + method->second;
	switch (id)
	{
		case Camera::CamEndpointID::epBayerOffsetX:
			pHadler = std::make_shared<Camera::Endpoint_BayerOffsetX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epBayerOffsetY:
			pHadler = std::make_shared<Camera::Endpoint_BayerOffsetY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epBinX:
			pHadler = std::make_shared<Camera::Endpoint_BinX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epBinY:
			pHadler = std::make_shared<Camera::Endpoint_BinY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCameraState:
			pHadler = std::make_shared<Camera::Endpoint_CameraState>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCameraXSize:
			pHadler = std::make_shared<Camera::Endpoint_CameraXSize>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCameraYSize:
			pHadler = std::make_shared<Camera::Endpoint_CameraYSize>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanAbortExposure:
			pHadler = std::make_shared<Camera::Endpoint_CanAbortExposure>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanAsymmetricBin:
			pHadler = std::make_shared<Camera::Endpoint_CanAsymmetricBin>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanFastReadout:
			pHadler = std::make_shared<Camera::Endpoint_CanFastReadout>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanGetCoolerPower:
			pHadler = std::make_shared<Camera::Endpoint_CanGetCoolerPower>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanPulseGuide:
			pHadler = std::make_shared<Camera::Endpoint_CanPulseGuide>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanSetCCDTemp:
			pHadler = std::make_shared<Camera::Endpoint_CanSetCCDTemp>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCanStopExposure:
			pHadler = std::make_shared<Camera::Endpoint_CanStopExposure>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCCDTemp:
			pHadler = std::make_shared<Camera::Endpoint_CCDTemp>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCoolerOn:
			pHadler = std::make_shared<Camera::Endpoint_CoolerOn>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCoolerPower:
			pHadler = std::make_shared<Camera::Endpoint_CoolerPower>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epElectronsPerAdu:
			pHadler = std::make_shared<Camera::Endpoint_ElectronsPerADU>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epExposureMin:
			pHadler = std::make_shared<Camera::Endpoint_ExposureMin>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epExposureMax:
			pHadler = std::make_shared<Camera::Endpoint_ExposureMax>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epExposureResolution:
			pHadler = std::make_shared<Camera::Endpoint_ExposureResolution>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epFastReadout:
			pHadler = std::make_shared<Camera::Endpoint_FastReadout>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epFullWellCapacity:
			pHadler = std::make_shared<Camera::Endpoint_FullWellCapacity>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epGain:
			pHadler = std::make_shared<Camera::Endpoint_Gain>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epGainMax:
			pHadler = std::make_shared<Camera::Endpoint_GainMax>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epGainMin:
			pHadler = std::make_shared<Camera::Endpoint_GainMin>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epGains:
			pHadler = std::make_shared<Camera::Endpoint_Gains>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epHasShutter:
			pHadler = std::make_shared<Camera::Endpoint_HasShutter>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epHeatsinkTemp:
			pHadler = std::make_shared<Camera::Endpoint_HeatSinkTemp>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epImageReady:
			pHadler = std::make_shared<Camera::Endpoint_ImageReady>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epImageArray:
			pHadler = std::make_shared<Camera::Endpoint_ImageArray>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epImageArrayVariant:
			pHadler = std::make_shared<Camera::Endpoint_ImageArrayVariant>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epIsPulseGuiding:
			pHadler = std::make_shared<Camera::Endpoint_IsPulseGuiding>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epLastExposureDuration:
			pHadler = std::make_shared<Camera::Endpoint_LastExposureDuration>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epLastExposureStartTime:
			pHadler = std::make_shared<Camera::Endpoint_LastExposureStartTime>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epMaxADU:
			pHadler = std::make_shared<Camera::Endpoint_MaxADU>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epMaxBinX:
			pHadler = std::make_shared<Camera::Endpoint_MaxBinX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epMaxBinY:
			pHadler = std::make_shared<Camera::Endpoint_MaxBinY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epNumX:
			pHadler = std::make_shared<Camera::Endpoint_NumX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epNumY:
			pHadler = std::make_shared<Camera::Endpoint_NumY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epOffset:
			pHadler = std::make_shared<Camera::Endpoint_Offset>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epOffsetMin:
			pHadler = std::make_shared<Camera::Endpoint_OffsetMin>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epOffsetMax:
			pHadler = std::make_shared<Camera::Endpoint_OffsetMax>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epOffsets:
			pHadler = std::make_shared<Camera::Endpoint_Offsets>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epPercentComplete:
			pHadler = std::make_shared<Camera::Endpoint_PercentComplete>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epPixelSizeX:
			pHadler = std::make_shared<Camera::Endpoint_PixelSizeX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epPixelSizeY:
			pHadler = std::make_shared<Camera::Endpoint_PixelSizeY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epReadoutMode:
			pHadler = std::make_shared<Camera::Endpoint_ReadoutMode>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epReadoutModes:
			pHadler = std::make_shared<Camera::Endpoint_ReadoutModes>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epSensorName:
			pHadler = std::make_shared<Camera::Endpoint_SensorName>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epSensorType:
			pHadler = std::make_shared<Camera::Endpoint_SensorType>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epCoolerSetpoint:
			pHadler = std::make_shared<Camera::Endpoint_CoolerSetpoint>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epStartX:
			pHadler = std::make_shared<Camera::Endpoint_StartX>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epStartY:
			pHadler = std::make_shared<Camera::Endpoint_StartY>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epSubExposureDuration:
			pHadler = std::make_shared<Camera::Endpoint_SubExposureDuration>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epAbortExposure:
			pHadler = std::make_shared<Camera::Endpoint_AbortExposure>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epPulseGuide:
			pHadler = std::make_shared<Camera::Endpoint_IsPulseGuiding>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epStartExposure:
			pHadler = std::make_shared<Camera::Endpoint_StartExposure>(fullEndpointString);
			break;
		case Camera::CamEndpointID::epStopExposure:
			pHadler = std::make_shared<Camera::Endpoint_StopExposure>(fullEndpointString);
			break;
	default:
		break;
	}

	return pHadler;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BayerOffsetX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getBayerOffsetX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BayerOffsetY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getBayerOffsetY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BinX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getBinX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BinX::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("BinX"))
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No BinX value Supplied");
		return pResponseBody;
	}

	auto binX = body.extract("BinX", 0);

	if (binX <= 0 || binX > pCamera->getMaxBinX())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied");
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

	pCamera->setBinX(binX);

	if (!pCamera->getCanAsymmetricBin())
	{
		pCamera->setBinY(binX);
	}

	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BinY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getBinY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_BinY::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto binY = body.extract("BinY", 0);

	if (binY <= 0 || binY > pCamera->getMaxBinY())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied");
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

	pCamera->setBinY(binY);

	if (!pCamera->getCanAsymmetricBin())
	{
		pCamera->setBinX(binY);
	}

	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CameraState::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	auto stateID = static_cast<int>(pCamera->getCameraState());

	pResponseBody->setValue(stateID);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CameraXSize::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCameraXSize());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CameraYSize::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCameraYSize());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanAbortExposure::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanAbortExposure());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanAsymmetricBin::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanAsymmetricBin());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanFastReadout::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanFastReadout());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanGetCoolerPower::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanGetCoolerPower());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanPulseGuide::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanPulseGuide());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanSetCCDTemp::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanSetCCDTemp());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CanStopExposure::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCanStopExposure());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CCDTemp::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCCDTemp());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CoolerOn::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCoolerOn());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CoolerOn::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("CoolerOn"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto coolerOn = body.extract("CoolerOn", false);

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

	pCamera->setCoolerOn(coolerOn);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CoolerPower::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCoolerPower());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ElectronsPerADU::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getElectrosPerAdu());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ExposureMax::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getExposureMax());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ExposureMin::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getExposureMin());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ExposureResolution::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getExposureResolution());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_FastReadout::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getFastReadout());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_FastReadout::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("FastReadout"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	if (!pCamera->getCanFastReadout())
	{
		pResponseBody->setErrorMessage("Property Not Supported");
		pResponseBody->setErrorNumber(ErrorCodes::c_notImplemented);
		return pResponseBody;
	}

	auto fastReadout = body.extract("FastReadout", false);

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
	
	pCamera->setFastReadout(fastReadout);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Gain::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getGain());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Gain::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("Gain"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto gain = body.extract("Gain", 0);
	
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

	if (gain < pCamera->getGainMin() || gain > pCamera->getGainMax())
	{
		pResponseBody->setErrorMessage("Invalid Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	pCamera->setGain(gain);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_GainMax::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getGainMax());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_GainMin::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getGainMin());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Gains::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::StringArrayResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getGains());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_HasShutter::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getHasShutter());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_HeatSinkTemp::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getHeatsinkTemp());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ImageArray::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::ImageResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	auto pImgData = pCamera->getImageArray();

	if (!pImgData)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Image Stored");
		return pResponseBody;
	}

	auto type = pImgData->getType();
	auto rank = pImgData->getRank();

	if (type != Image::ImageType::Int32)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Array Pixel Type");
		return pResponseBody;
	}

	pResponseBody->setRank(rank);
	pResponseBody->setType(type);

	pResponseBody->setImageData(pCamera->getImageArray());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ImageArrayVariant::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::ImageResponse>();
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID      = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	if (!pCamera->getConnected())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_notConnected);
		pResponseBody->setErrorMessage("Image is not valid");
		return pResponseBody;
	}

	auto pImgData = pCamera->getImageArray();

	if (!pImgData)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Image Stored");
		return pResponseBody;
	}
	if (!pImgData->validate())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("Image is not valid");
		return pResponseBody;
	}

	auto type = pImgData->getType();
	auto rank = pImgData->getRank();

	pResponseBody->setRank(rank);
	pResponseBody->setType(type);

	pResponseBody->setImageData(pCamera->getImageArray());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ImageReady::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getImageReady());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_IsPulseGuiding::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::BoolResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getIsPulseGuiding());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_LastExposureDuration::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	auto lastDuration = pCamera->getLastExposureDuration();
	if (lastDuration < 0)
	{
		pResponseBody->setErrorMessage("No Exposure Has Been Started Yet");
		pResponseBody->setErrorNumber(ErrorCodes::c_unspecifiedError);
		return pResponseBody;
	}

	pResponseBody->setValue(lastDuration);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_LastExposureStartTime::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	auto lastStartTime = pCamera->getLastExposureStartTime();
	if (lastStartTime.empty())
	{
		pResponseBody->setErrorMessage("No Exposure Has Been Started Yet");
		pResponseBody->setErrorNumber(ErrorCodes::c_unspecifiedError);
		return pResponseBody;
	}

	pResponseBody->setValue(lastStartTime);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_MaxADU::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getMaxADU());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_MaxBinX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getMaxBinX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_MaxBinY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getMaxBinY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_NumX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getNumX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_NumX::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("NumX"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto numX = body.extract("NumX", -1);
	if (numX < 0)
	{
		pResponseBody->setErrorMessage("Invalid Value Supplied");
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

	if (numX <= 0 || numX > pCamera->getCameraXSize())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied: Subframe X value out of bounds");
		return pResponseBody;
	}

	pCamera->setNumX(numX);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_NumY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getNumY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_NumY::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("NumY"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto numY = body.extract("NumY", -1);
	if (numY < 0)
	{
		pResponseBody->setErrorMessage("Invalid Value Supplied");
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

	if (numY <= 0 || numY > pCamera->getCameraYSize())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied: Subframe X value out of bounds");
		return pResponseBody;
	}

	pCamera->setNumY(numY);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Offset::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getOffset());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Offset::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("Offset"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto offset = body.extract("Offset", 0);

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

	if (offset < pCamera->getOffsetMin() || offset > pCamera->getOffsetMax())
	{
		pResponseBody->setErrorMessage("Invalid Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	pCamera->setOffset(offset);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_OffsetMin::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getOffsetMin());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_OffsetMax::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getOffsetMax());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_Offsets::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::StringArrayResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getOffsets());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_PercentComplete::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getPercentComplete());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_PixelSizeX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getPixelSizeX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_PixelSizeY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getPixelSizeY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_FullWellCapacity::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getFullWellCapacity());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ReadoutMode::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getReadoutMode());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ReadoutMode::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}


	if (!body.contains("Gain"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto mode = body.extract("ReadoutMode", 0);

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

	auto modes = pCamera->getReadoutModes();
	if (mode < 0 || mode > modes.size())
	{
		pResponseBody->setErrorMessage("Invalid Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	pCamera->setReadoutMode(mode);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_ReadoutModes::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::StringArrayResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getReadoutModes());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_SensorName::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::StringResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getSensorName());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_SensorType::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	auto type = static_cast<int>(pCamera->getSensorType());

	pResponseBody->setValue(type);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CoolerSetpoint::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::DoubleResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getCoolerSetpoint());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_CoolerSetpoint::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("SetCCDTemperature"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto setpoint = body.extract("SetCCDTemperature", pCamera->getCCDTemp());

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

	if (pCamera->getCanSetCCDTemp())
	{
		pCamera->setCoolerSetpoint(setpoint);
	}
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}


std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StartX::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getStartX());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StartX::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("StartX"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto startX = body.extract("StartX", 0);

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

	if (startX < 0 || startX > pCamera->getCameraXSize())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied: Start X value out of bounds");
		return pResponseBody;
	}

	pCamera->setStartX(startX);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}
std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StartY::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getStartY());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StartY::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("StartY"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto startY = body.extract("StartY", 0);

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

	if (startY < 0 || startY > pCamera->getCameraYSize())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Value Supplied: Start X value out of bounds");
		return pResponseBody;
	}


	pCamera->setStartY(startY);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_SubExposureDuration::handleRequest_Get(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::IntResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	auto clientID = queries.extract(Common::c_paramKeyClientID, -1);
	auto transactionID = queries.extract(Common::c_paramKeyClientTransactionID, -1);
	if (transactionID > 0)
	{
		pResponseBody->setClientTransactionID(++transactionID);
	}

	pResponseBody->setValue(pCamera->getSubExposureDuration());
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_SubExposureDuration::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (!body.contains("SubExposureDuration"))
	{
		pResponseBody->setErrorMessage("No Value Supplied");
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		return pResponseBody;
	}

	auto subExposureDuration = body.extract("SubExposureDuration", -1.0);

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

	if (subExposureDuration < 0)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Subexposure Duration Provided");
		return pResponseBody;
	}

	pCamera->setSubExposureDuration(subExposureDuration);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_AbortExposure::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

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

	if (!pCamera->getCanAbortExposure())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_notImplemented);
		pResponseBody->setErrorMessage("Abort Exposure Not Supported");
		return pResponseBody;
	}

	pCamera->abortExposure();
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_PulseGuide::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Request Body Supplied");
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

	if (!pCamera->getCanPulseGuide())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_notImplemented);
		pResponseBody->setErrorMessage("Pulse Guiding Not Supported");
		return pResponseBody;
	}

	if (!body.contains("Direction"))
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Pulse Guide Direction Supplied");
		return pResponseBody;
	}
	if (!body.contains("Duration"))
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Pulse Guide Duration Supplied");
		return pResponseBody;
	}

	auto pulseGuideDirection = static_cast<Camera::PulseGuideDirection>(body.extract("Direction", -1));
	auto pulseGuideDuration  = body.extract("Duration", -1);
	
	if (pulseGuideDirection == Camera::PulseGuideDirection::Unknown)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Pulse Guide Direction");
		return pResponseBody;
	}

	if (pulseGuideDuration < 0)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Pulse Guide Duration");
		return pResponseBody;
	}

	if (pCamera->getIsPulseGuiding())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_unspecifiedError);
		pResponseBody->setErrorMessage("Camera Already Pulse Guiding");
		return pResponseBody;
	}

	pCamera->pulseGuide(pulseGuideDirection, pulseGuideDuration);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StartExposure::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Request Body Supplied");
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

	if (!body.contains("Duration"))
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Duration Supplied");
		return pResponseBody;
	}
	if (!body.contains("Light"))
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Light Frame Parameter Supplied");
		return pResponseBody;
	}

	auto duration = body.extract("Duration", -1.0);
	auto isLight  = body.extract("Light", false);

	if (duration < 0)
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		pResponseBody->setErrorMessage("Invalid Exposure Duration Provided");
		return pResponseBody;
	}

	if (pCamera->getDeviceBusy())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_unspecifiedError);
		pResponseBody->setErrorMessage("Device Is Busy");
		return pResponseBody;
	}

	pCamera->startExposure(duration, isLight);
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	return pResponseBody;
}

std::shared_ptr<JSONInfoBody> Alpaca::Camera::Endpoint_StopExposure::handleRequest_Put(std::string target, ParameterMap queries, ParameterMap body, TRESTCtxPtr pCtx)
{
	auto pCamera = std::dynamic_pointer_cast<IAlpacaCamV1>(pCtx);
	if (!pCamera)
	{
		throw std::runtime_error("Failed to retrieve camera");
	}

	auto pResponseBody = std::make_shared<Bodies::MethodResponse>();
	pResponseBody->setServerTransactionID(pCamera->getNextransactionID());

	auto deviceNum = extractDeviceNum(target);
	if (deviceNum < 0)
	{
		pResponseBody->setErrorMessage("Invalid Device Type");
		pResponseBody->setErrorNumber(ErrorCodes::c_invalidValue);
		return pResponseBody;
	}

	if (body.empty())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_valueNotSet);
		pResponseBody->setErrorMessage("No Request Body Supplied");
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

	if (!pCamera->getCanStopExposure())
	{
		pResponseBody->setErrorNumber(ErrorCodes::c_notImplemented);
		pResponseBody->setErrorMessage("Stop Exposure Not Supported");
		return pResponseBody;
	}

	pCamera->stopExposure();
	return pResponseBody;
}

#include "stdafx.h"
#include "IAlpacaCamV1.h"
#include "AlpacaCamEndpointsV1.h"

using namespace Alpaca;
using namespace Alpaca::Camera;

IAlpacaCamV1::IAlpacaCamV1(unsigned int deviceNum, std::string deviceName)
	: AlpacaDeviceV1(AlpacaDeviceV1::DeviceTypeID::Camera, deviceNum, deviceName)
	, m_bayerOffsetX(0)
	, m_bayerOffsetY(0)
	, m_binX(1)
	, m_binY(1)
	, m_cameraState(CameraStateID::CameraIdle)
	, m_cameraXSize(512)
	, m_cameraYSize(512)
	, m_canAbortExposure(false)
	, m_canAsymmetricBin(false)
	, m_canFastReadout(false)
	, m_canGetCoolerPower(false)
	, m_canPulseGuide(false)
	, m_canSetCCDTemp(false)
	, m_canStopExposure(false)
	, m_CCDTemp(0.0)
	, m_coolerOn(false)
	, m_coolerPower(0.0)
	, m_electrosPerAdu(1.0)
	, m_exposureMin(0.0001)
	, m_exposureMax(900.0)
	, m_eposureResolution(0.0001)
	, m_fastReadout(false)
	, m_fullWellCapacity(0.0)
	, m_gain(0)
	, m_gainMax(4)
	, m_gainMin(0)
	, m_gains({"0", "1", "2", "3"})
	, m_hasShutter(false)
	, m_heatsinkTemp(0.0)
	, m_imageReady(false)
	, m_isPulseGuiding(false)
	, m_lastExposureDuration(-1.0)
	, m_lastExposureStartTime("")
	, m_maxADU(INT32_MAX)
	, m_maxBinX(4)
	, m_maxBinY(4)
	, m_numX(m_cameraXSize)
	, m_numY(m_cameraYSize)
	, m_offset(0)
	, m_offsetMin(0)
	, m_offsetMax(4)
	, m_offsets({ "0", "1", "2", "3" })
	, m_percentComplete(0)
	, m_pixelSizeX(1.0)
	, m_pixelSizeY(1.0)
	, m_readoutMode(0)
	, m_readoutModes({ "0", "1", "2", "3" })
	, m_sensorName("Simulated Sensor")
	, m_sensorType(SensorTypeID::Mono)
	, m_coolerSetpoint(0.0)
	, m_startX(0)
	, m_startY(0)
	, m_subExposureDuration(0.0)
	, m_pImageArray()
{
	m_pImageArray = std::make_shared<Image::ImageData>();
	unsigned short* pBuf = new unsigned short[m_numX * m_numY];
	Image::ImagePlane plane(pBuf, m_numX * m_numY, Image::ImageType::Int16);
	m_pImageArray->addPlane(plane);

	initCamHandlers();
}	 

IAlpacaCamV1::~IAlpacaCamV1()
{
}

void Alpaca::Camera::IAlpacaCamV1::initCamHandlers()
{
	try
	{
		std::shared_ptr<HTTPRequestHandler> pHandler = nullptr;

		auto listStart = static_cast<unsigned int>(Alpaca::Camera::CamEndpointID::epFirst);
		auto listEnd   = static_cast<unsigned int>(Alpaca::Camera::CamEndpointID::epLast);


		std::ostringstream oss;
		oss << m_rootEPName << "/" << "camera" << "/" << m_deviceNumber;

		for (auto i = listStart; i < listEnd; i++)
		{
			auto id = static_cast<Alpaca::Camera::CamEndpointID>(i);
			pHandler = Alpaca::Camera::createHandler(id, oss.str());

			if (pHandler)
			{
				registerHandler(pHandler->getName(), pHandler);
			}
		}
	}
	catch (std::exception& err)
	{
		throw std::runtime_error("Failed to initialize common endpoints");
	}
}

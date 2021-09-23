#include "stdafx.h"
#include "CameraV1.h"
#include "CamEvents.h"
#include <random>

using namespace Camera;
using namespace Camera::Event;

const std::string c_resourceRoot = "C:\\Users\\xs_br\\source\\repos\\branthony-92\\ASCOM\\Alpaca\\AlpacaCameraSimV1";

CameraV1::CameraV1(unsigned int deviceIndex, std::string name)
	: IAlpacaCamV1(deviceIndex, name)
	, CSMStateContext(name)
	, m_generateBackground(true)
	, m_bufferReadOffset(0)
	, m_imageType(Alpaca::Image::ImageType::Int32)
{
	m_description = "Alpaca Camera Simulator";
	m_driverInfo = "Test driver";
	m_supportedActions = {
		"Action 1",
		"Action 2"
	};
	m_resourceRoot = c_resourceRoot;
}

CameraV1::~CameraV1()
{
}

void Camera::CameraV1::onServerStart()
{

}

void Camera::CameraV1::onServerReset()
{
	disconnect();
}

void Camera::CameraV1::onServerShutdown()
{
	disconnect();
}

std::string Camera::CameraV1::findResource(std::string target)
{
	std::string path = "";

	std::ostringstream oss;
	oss << "/setup/v1/camera/" << m_deviceNumber << "/setup";

	auto deviceSetupTarget = oss.str();

	if (target == "/")
	{
		path = m_resourceRoot + "\\resources\\test\\index.html";
	}
	if (target == deviceSetupTarget)
	{
		// configure this device
		path = m_resourceRoot + "\\resources\\setup\\camera_sim_v1\\device_setup.html";
	}
	if (target == "/setup" )
	{
		// configure for all devices (shared between all devices registered as contexts)
		path = m_resourceRoot + "\\resources\\setup\\general_device_setup.html";
	}

	return path;
}

std::string CameraV1::action(std::string action, std::string parameters)
{
	return std::string();
}

void CameraV1::commandBlind(std::string cmd, std::string raw)
{
}

bool CameraV1::commandBool(std::string cmd, std::string raw)
{
	return false;
}

std::string CameraV1::commandString(std::string cmd, std::string raw)
{
	return std::string();
}

void CameraV1::startExposure(double duration, bool isLight)
{
	m_imageReady = false;
	m_lastExposureDuration = duration;
	m_isLight = isLight;

	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);

	auto gm = gmtime(&t);
	char timestampBuf[80];

	strftime(timestampBuf, 80, "%F%X", gm);
	m_lastExposureStartTime = timestampBuf;

	postEvent(create<EventID::CamEvent_StartExposure>());
	m_cameraState = Alpaca::Camera::CameraStateID::CameraExposing;

	std::string status = "Camera Connected - Exposing...";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
}

void CameraV1::stopExposure()
{
	postEvent(create<EventID::CamEvent_StopExposure>());
}

void CameraV1::abortExposure()
{
	postEvent(create<EventID::CamEvent_AbortExposure>());
	std::string status = "Camera Connected - Aborting Exposure...";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
}

void CameraV1::pulseGuide(Alpaca::Camera::PulseGuideDirection direction, int duration)
{
	postEvent(create<EventID::CamEvent_PulseGuide>());
	std::string status = "Camera Connected - Pulse Guiding...";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
}

void Camera::CameraV1::connect()
{
	if (!m_connected)
	{
		std::string status = "Camera Connecting...";
		updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
		postEvent(Event::create<EventID::CamEvent_Connect>());
	}
}

void Camera::CameraV1::setupCamera()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
	std::string status = "Camera Connected";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
	m_connected = true;
}

void Camera::CameraV1::disconnect()
{
	if (m_connected)
	{
		postEvent(Event::create<EventID::CamEvent_Disconnect>());
		std::string status = "Camera Disconnecting...";
		updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
	}
}

void Camera::CameraV1::shutdownCamera()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
	std::string status = "Camera Not Connected";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
	m_connected = false;
}

void Camera::CameraV1::finishExposure()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
}

void Camera::CameraV1::recoverState()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
}

void Camera::CameraV1::onStart()
{
	std::string status = "Camera Not Connected";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
}

void Camera::CameraV1::onIdle()
{
	setCameraState(Alpaca::Camera::CameraStateID::CameraIdle);
	std::string status = "Camera Connected - Idle";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
	m_percentComplete = 0;
}



void Camera::CameraV1::initImg2D()
{
	auto xSize = m_cameraXSize;
	auto ySize = m_cameraYSize;

	auto len = xSize * ySize;
	std::default_random_engine generator(time(NULL));
	std::normal_distribution<double> distribution(100 * m_lastExposureDuration, 10 * m_lastExposureDuration);

	auto pBuffer = std::make_shared<Alpaca::Image::Buffer>(len, m_imageType);
	for (auto i = 0; i < len; i++)
	{
		switch (m_imageType)
		{
		case Alpaca::Image::ImageType::Int16:
			pBuffer->getData()[i].i16 = static_cast<short>(distribution(generator));
			break;
		case Alpaca::Image::ImageType::Int32:
			pBuffer->getData()[i].i32 = static_cast<int>(distribution(generator));
			break;
		case Alpaca::Image::ImageType::Float64:
			pBuffer->getData()[i].f64 = static_cast<float>(distribution(generator));
			break;
		default:
			break;
		}

	}
	Alpaca::Image::ImagePlane plane;
	plane.setImageBuffer(pBuffer);
	m_pImageArray->addPlane(plane);

	m_pImageArray->setRank(2);
	m_pImageArray->setType(m_imageType);

	m_pImageArray->setXSize(xSize);
	m_pImageArray->setYSize(ySize);
}

void Camera::CameraV1::initImg3D()
{
	auto xSize = m_cameraXSize;
	auto ySize = m_cameraYSize;

	auto len = xSize * ySize;
	std::default_random_engine generator(time(NULL));

	std::normal_distribution<double> distribution(100 * m_lastExposureDuration, 10 * m_lastExposureDuration);

	auto pBuffer1 = std::make_shared<Alpaca::Image::Buffer>(len, m_imageType);
	auto pBuffer2 = std::make_shared<Alpaca::Image::Buffer>(len, m_imageType);
	auto pBuffer3 = std::make_shared<Alpaca::Image::Buffer>(len, m_imageType);
	Alpaca::Image::ImagePlane plane1;
	Alpaca::Image::ImagePlane plane2;
	Alpaca::Image::ImagePlane plane3;

	for (auto i = 0; i < len; i++)
	{
		switch (m_imageType)
		{
		case Alpaca::Image::ImageType::Int16:
			pBuffer1->getData()[i].i16 = static_cast<short>(distribution(generator));
			pBuffer2->getData()[i].i16 = static_cast<short>(distribution(generator));
			pBuffer3->getData()[i].i16 = static_cast<short>(distribution(generator));
			break;
		case Alpaca::Image::ImageType::Int32:
			pBuffer1->getData()[i].i32 = static_cast<int>(distribution(generator));
			pBuffer2->getData()[i].i32 = static_cast<int>(distribution(generator));
			pBuffer3->getData()[i].i32 = static_cast<int>(distribution(generator));
			break;
		case Alpaca::Image::ImageType::Float64:
			pBuffer1->getData()[i].f64 = static_cast<float>(distribution(generator));
			pBuffer2->getData()[i].f64 = static_cast<float>(distribution(generator));
			pBuffer3->getData()[i].f64 = static_cast<float>(distribution(generator));
			break;
		default:
			break;
		}
	}
	plane1.setImageBuffer(pBuffer1);
	m_pImageArray->addPlane(plane1);

	plane2.setImageBuffer(pBuffer2);
	m_pImageArray->addPlane(plane2);

	plane3.setImageBuffer(pBuffer3);
	m_pImageArray->addPlane(plane3);

	m_pImageArray->setRank(3);
	m_pImageArray->setType(m_imageType);

	m_pImageArray->setXSize(xSize);
	m_pImageArray->setYSize(ySize);
}

void Camera::CameraV1::generateImgBackground()
{
	auto delaySec = static_cast<int>(m_lastExposureDuration);
	std::this_thread::sleep_for(std::chrono::seconds(delaySec));
	m_cameraState = Alpaca::Camera::CameraStateID::CameraReading;
	std::string status = "Camera Connected - Generating Simulated Image...";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));

	m_pImageArray = std::make_shared<Alpaca::Image::ImageData>();

	switch (m_sensorType)
	{
		case Alpaca::Camera::SensorTypeID::Mono:
		case Alpaca::Camera::SensorTypeID::RGGB:
		case Alpaca::Camera::SensorTypeID::CMYG:
		case Alpaca::Camera::SensorTypeID::CMYG2:
		case Alpaca::Camera::SensorTypeID::LRGB:
			initImg2D();
			break;	
		case Alpaca::Camera::SensorTypeID::Colour:
			initImg3D();
			break;
		default:
			break;
	}
}


void CameraV1::generateStar(const unsigned int planeIndex, const unsigned int starX, const unsigned int starY)
{
	//calculate x and y shift
	const unsigned int c_starWidth = 4u;

	auto xSize = m_pImageArray->getXSize(); 
	auto ySize = m_pImageArray->getYSize();

	// if the star is shifted out of frame by the pulse guiding don't draw it
	double w = -1.0 / (2.0 * c_starWidth * c_starWidth);
	double maxIntensity = 10000 * m_lastExposureDuration;
	auto span = 4u * c_starWidth;
	auto xi = starX - span;
	auto xf = starX + span;
	auto yi = starY - span;
	auto yf = starY + span;
	auto radius = c_starWidth;
	auto totalRadius = (xf - xi) / 2;

	auto plane = m_pImageArray->getPlanes()[planeIndex];
	auto pBuf = plane.getImageBuffer()->getData();

	//draw the star
	for (size_t j = yi; j < yf; j++)
	{
		for (size_t i = xi; i < xf; i++)
		{
			auto _y = j - static_cast<unsigned int>(starX);
			auto _x = i - static_cast<unsigned int>(starY);
			auto _r = static_cast<double>(_x * _x + _y * _y);

			double val = 0.0;
			if (_r <= radius * radius)
			{
				val = maxIntensity;
			}
			if (_r > radius * radius && _r <= totalRadius * totalRadius)
			{
				val = maxIntensity * exp(w * _r);
			}

			switch (m_pImageArray->getType())
			{
			case Alpaca::Image::ImageType::Int16:
				pBuf[i + j * xSize].i16 = static_cast<short>(val);
				break;
			case Alpaca::Image::ImageType::Int32:
				pBuf[i + j * xSize].i32 = static_cast<int>(val);
				break;
			case Alpaca::Image::ImageType::Float64:
				pBuf[i + j * xSize].f64 = static_cast<double>(val);
				break;
			default:
				break;
			}
		}
	}
}

void CameraV1::generateImage()
{
	m_cameraState = Alpaca::Camera::CameraStateID::CameraReading;

	auto planeCount = m_pImageArray->getPlanes().size();
	auto xSize = m_pImageArray->getXSize();
	auto ySize = m_pImageArray->getYSize();

	if (m_isLight)
	{
		// draw a single star in the middle of the frame
		const auto centerX = xSize / 2;
		const auto centerY = ySize / 2;

		const auto topRightX = centerX + xSize / 4;
		const auto topRightY = centerY - ySize / 4;

		const auto bottomRightX = centerX + xSize / 4;
		const auto bottomRightY = centerY + ySize / 4;

		const auto bottomLeftX = centerX - xSize / 4;
		const auto bottomLeftY = centerY + ySize / 4;

		const auto topLeftX = centerX - xSize / 4;
		const auto topLeftY = centerY - ySize / 4;

		for (auto i = 0u ; i < planeCount; i++)
		{
			generateStar(i, centerX,      centerY);
			//generateStar(i, topRightX,    topRightY);
			//generateStar(i, bottomRightX, bottomRightY);
			//generateStar(i, bottomLeftX,  bottomLeftY);
			//generateStar(i, topLeftX,     topLeftY);
		}
	}

	// bin if needed
	if (m_binX != 1 || m_binY != 1)
	{
		applyBinning();
	}
	reframeImg();
	m_imageReady = true;
}

void Camera::CameraV1::reframeImg()
{
	
}


void Camera::CameraV1::applyBinning()
{
	using namespace Alpaca::Image;
	auto binPixels = [this](const std::shared_ptr<Buffer> pBuf, const int x, const int y)
	{
		double sum = 0;

		// ensure our mask stays in range
		const int startX = std::max(0, x);
		const int startY = std::max(0, y);
		const int endX   = std::min((x + m_binX), m_cameraXSize);
		const int endY   = std::min((y + m_binY), m_cameraYSize);

		for (auto _y = startY; _y < endY; _y++)
		{
			for (auto _x = startX; _x < endX; _x++)
			{
				switch (pBuf->getType())
				{
				case Alpaca::Image::ImageType::Int16:
					sum += pBuf->getData()[_x + _y * m_cameraXSize].i16;
					break;
				case Alpaca::Image::ImageType::Int32:
					sum += pBuf->getData()[_x + _y * m_cameraXSize].i32;
					break;
				case Alpaca::Image::ImageType::Float64:
					sum += pBuf->getData()[_x + _y * m_cameraXSize].f64;
					break;
				default:
					break;
				}
			}
		}
		unsigned int numPixels = m_binX * m_binY;

		Buffer::VarPixel avg;
		sum = sum / numPixels;
		switch (pBuf->getType())
		{
		case Alpaca::Image::ImageType::Int16:
			sum = std::min(sum, static_cast<double>(INT32_MAX));
			avg.i16 = static_cast<int>(sum);
			break;
		case Alpaca::Image::ImageType::Int32:
			sum = std::min(sum, static_cast<double>(INT32_MAX));
			avg.i32 = static_cast<int>(sum);
			break;
		case Alpaca::Image::ImageType::Float64:
			sum = std::min(sum, DBL_MAX);
			avg.f64 = sum;
			break;
		default:
			avg.i16 = 0;
			break;
		}
		// clamp to the max pixel value to prevent overflows
		return avg;
	};

	auto planes = m_pImageArray->getPlanes();

	for (auto& plane : planes)
	{
		const unsigned int xTarget = m_numX;
		const unsigned int yTarget = m_numY;

		const unsigned int targetLen = xTarget * yTarget;

		const unsigned int xEnd = m_cameraXSize;
		const unsigned int yEnd = m_cameraYSize;

		const unsigned int xSkip = m_binX;
		const unsigned int ySkip = m_binY;

		unsigned int outX = 0;
		unsigned int outY = 0;

		auto pSourceBuffer = plane.getImageBuffer();
		auto pDestBuffer = std::make_shared<Alpaca::Image::Buffer>(targetLen, pSourceBuffer->getType());

		for (auto y = 0u; y < yEnd; y += ySkip)
		{
			outX = 0;
			for (auto x = 0u; x < xEnd; x += xSkip)
			{
				pDestBuffer->getData()[outX + outY * xTarget] = binPixels(pSourceBuffer, x, y);
				if (++outX >= xTarget) break;
			}
			if (++outY >= yTarget) break;
		}

		plane.setImageBuffer(pDestBuffer);
	}
	m_pImageArray->setPlanes(planes);
	m_pImageArray->setXSize(m_numX);
	m_pImageArray->setYSize(m_numY);
}

void Camera::CameraV1::transferImage()
{
	m_cameraState = Alpaca::Camera::CameraStateID::CameraDownload;
	std::string status = "Camera Connected - Transferring...";
	updateViews(Hint::UpdateCamStatusHint(Hint::DeviceViewData(getDeviceID(), status)));
}

bool Camera::CameraV1::readChunk(uint32_t chunkSize)
{
	bool done = false;

	auto xSizeBinned = m_numX;
	auto ySizeBinned = m_numY;
	auto len = xSizeBinned * ySizeBinned;

	auto chunkEndPos = m_bufferReadOffset + chunkSize;
	if (chunkEndPos >= len)
	{
		done = true;
		chunkEndPos = len;
	}

	for (auto plane : m_pImageArray->getPlanes())
	{
		auto pSrcBuf = plane.getImageBuffer();
		auto pDestBuf = std::make_shared<Alpaca::Image::Buffer>(len, pSrcBuf->getType());

		for (auto i = m_bufferReadOffset; i < chunkEndPos; i++)
		{
			pDestBuf->getData()[i] = pSrcBuf->getData()[i];
		}
	}
	m_bufferReadOffset = chunkEndPos;
	float percent = static_cast<float>(m_bufferReadOffset / len);
	m_percentComplete = static_cast<int>(100.0f * percent);
	if (done)
	{
		postEvent(Event::create<EventID::CamEvent_Success>());
		m_percentComplete = 100;
	}
	return done;
}

Camera::ExposureManager::~ExposureManager()
{
	if (m_worker.joinable()) m_worker.join();
}

void Camera::ExposureManager::onGenerateBackground()
{
	if (m_worker.joinable()) m_worker.join();
	m_worker = std::thread([this]
	{
		m_pCam->generateImgBackground();
		postEvent(Event::create<EventID::CamEvent_Success>());
	});
}

void Camera::ExposureManager::onGenerateImg()
{
	if (m_worker.joinable()) m_worker.join();
	m_worker = std::thread([this]
	{
		m_pCam->generateImage();
		postEvent(Event::create<EventID::CamEvent_Success>());
		m_isComplete = true;
	});
}


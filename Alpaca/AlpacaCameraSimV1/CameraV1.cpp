#include "stdafx.h"
#include "CameraV1.h"
#include "CamEvents.h"
#include <random>

using namespace Camera;
using namespace Camera::Event;
CameraV1::CameraV1(unsigned int deviceIndex)
	: IAlpacaCamV1(deviceIndex, "Alpaca_Camera_Simulator_V1")
	, CSMStateContext("Alpaca_Camera_Simulator_V1")
	, m_generateBackground(true)
	, m_bufferReadOffset(0)
{
	m_description = "Alpaca Camera Simulator";
	m_driverInfo = "Test driver";
	m_supportedActions = {
		"Action 1",
		"Action 2"
	};

}

CameraV1::~CameraV1()
{
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
	m_lastExposureDuration = duration;
	m_isLight = isLight;
	postEvent(create<EventID::CamEvent_StartExposure>());
	m_cameraState = Alpaca::Camera::CameraStateID::CameraExposing;

	std::string status = "Camera Connected - Exposing...";
	updateViews(Hint::UpdateCamStatusHint(status));
}

void CameraV1::stopExposure()
{
	postEvent(create<EventID::CamEvent_StopExposure>());
}

void CameraV1::abortExposure()
{
	postEvent(create<EventID::CamEvent_AbortExposure>());
	std::string status = "Camera Connected - Aborting Exposure...";
	updateViews(Hint::UpdateCamStatusHint(status));
}

void CameraV1::pulseGuide(Alpaca::Camera::PulseGuideDirection direction, int duration)
{
	postEvent(create<EventID::CamEvent_PulseGuide>());
	std::string status = "Camera Connected - Pulse Guiding...";
	updateViews(Hint::UpdateCamStatusHint(status));
}

void Camera::CameraV1::connect()
{
	std::string status = "Camera Connecting...";
	updateViews(Hint::UpdateCamStatusHint(status));

	postEvent(Event::create<EventID::CamEvent_Connect>());
}

void Camera::CameraV1::disconnect()
{
	postEvent(Event::create<EventID::CamEvent_Disconnect>());
	std::string status = "Camera Disconnecting...";
	updateViews(Hint::UpdateCamStatusHint(status));
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
	updateViews(Hint::UpdateCamStatusHint(status));
}

void Camera::CameraV1::onIdle()
{
	setCameraState(Alpaca::Camera::CameraStateID::CameraIdle);
	std::string status = "Camera Connected - Idle";
	updateViews(Hint::UpdateCamStatusHint(status));
	m_percentComplete = 0;
}

void Camera::CameraV1::setupCamera()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
	std::string status = "Camera Connected";
	updateViews(Hint::UpdateCamStatusHint(status));
}

void Camera::CameraV1::shutdownCamera()
{
	postEvent(Event::create<EventID::CamEvent_Success>());
	std::string status = "Camera Not Connected";
	updateViews(Hint::UpdateCamStatusHint(status));
}


void Camera::CameraV1::generateImgBackground()
{
	auto delaySec = static_cast<int>(m_lastExposureDuration);
	std::this_thread::sleep_for(std::chrono::seconds(delaySec));
	m_cameraState = Alpaca::Camera::CameraStateID::CameraReading;
	std::string status = "Camera Connected - Generating Simulated Image...";
	updateViews(Hint::UpdateCamStatusHint(status));

	auto xSize = m_numX;
	auto ySize = m_numY;

	auto len = xSize * ySize;

	auto pBuffer = new int[len];

	std::default_random_engine generator(time(NULL));
	std::normal_distribution<double> distribution(1000, 50);

	for (auto i = 0; i < len; i++)
	{
		pBuffer[i] = static_cast<int>(distribution(generator));
	}
	m_pImageArray = std::make_shared< Alpaca::Image::ImageData>();
	m_pImageArray->setRank(2);
	m_pImageArray->setType(Alpaca::Image::ImageType::Int32);
	m_pImageArray->addPlane(Alpaca::Image::ImagePlane(pBuffer, len, Alpaca::Image::ImageType::Int32));
	m_pImageArray->setXSize(xSize);
	m_pImageArray->setYSize(ySize);
	delete[] pBuffer;
}


void CameraV1::generateStar(const unsigned int planeIndex, const unsigned int starX, const unsigned int starY)
{
	//calculate x and y shift
	const unsigned int c_starWidth = 5;

	auto xSize = m_pImageArray->getXSize(); 
	auto ySize = m_pImageArray->getYSize();


	// if the star is shifted out of frame by the pulse guiding don't draw it
	double w = -1.0 / (2.0 * c_starWidth * c_starWidth);
	double maxIntensity = 50000;
	auto span = 4u * c_starWidth;
	auto xi = std::max<unsigned int>(0, starX - span);
	auto xf = std::min<unsigned int>(starX + span, xSize);
	auto yi = std::max<unsigned int>(0, starY - span);
	auto yf = std::min<unsigned int>(starY + span, ySize);
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

			int val = 0;
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
				pBuf[i + j * xSize].i16 = val;
				break;
			case Alpaca::Image::ImageType::Int32:
				pBuf[i + j * xSize].i32 = val;
				break;
			case Alpaca::Image::ImageType::Float64:
				pBuf[i + j * xSize].f64 = val;
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

	for (auto i = 0u ; i < planeCount; i++)
	{
		// draw a single star in the middle of the frame
		const auto centerX = m_cameraXSize / 2;
		const auto centerY = m_cameraYSize / 2;

		generateStar(i, centerX, centerY);
	}

	// bin if needed
	if (m_binX != 1 || m_binY != 1)
	{
		switch (m_pImageArray->getType())
		{
		case Alpaca::Image::ImageType::Int16:
			applyBinning<short>();
			break;
		case Alpaca::Image::ImageType::Int32:
			applyBinning<int>();
			break;
		case Alpaca::Image::ImageType::Float64:
			applyBinning<double>();
			break;
		default:
			break;
		}

		m_pImageArray->setXSize(m_numX / m_binX);
		m_pImageArray->setXSize(m_numY / m_binY);
	}
	m_imageReady = true;
}



void Camera::CameraV1::transferImage()
{
	m_cameraState = Alpaca::Camera::CameraStateID::CameraDownload;
	std::string status = "Camera Connected - Transferring...";
	updateViews(Hint::UpdateCamStatusHint(status));

	m_imageReady = false;
}

bool Camera::CameraV1::readChunk(uint32_t chunkSize)
{
	bool done = false;

	auto xSizeBinned = m_numX / m_binX;
	auto ySizeBinned = m_numY / m_binY;
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


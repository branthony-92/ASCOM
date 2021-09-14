#include "stdafx.h"
#include "CameraV1.h"
#include "CamEvents.h"

using namespace Camera;
using namespace Camera::Event;
CameraV1::CameraV1(unsigned int deviceIndex)
	: IAlpacaCamV1(deviceIndex, "Alpaca_Camera_Simulator_V1")
	, CSMStateContext("Alpaca_Camera_Simulator_V1")
	, m_generateBackground(false)
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
	postEvent(Event::create<EventID::CamEvent_Connect>());

	std::string status = "Camera Connecting...";
	updateViews(Hint::UpdateCamStatusHint(status));
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
	m_cameraState = Alpaca::Camera::CameraStateID::CameraReading;
	postEvent(Event::create<EventID::CamEvent_Success>());
}

void CameraV1::generateImage()
{
	
	m_cameraState = Alpaca::Camera::CameraStateID::CameraReading;

}

void Camera::CameraV1::transferImage()
{
	m_cameraState = Alpaca::Camera::CameraStateID::CameraDownload;
	postEvent(Event::create<EventID::CamEvent_Success>());
}

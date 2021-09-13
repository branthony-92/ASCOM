#include "stdafx.h"
#include "CameraV1.h"

CameraV1::CameraV1(unsigned int deviceIndex)
	: IAlpacaCamV1(deviceIndex, "Alpaca_Camera_Simulator_V1")
	, CSMStateContext("Alpaca_Camera_Simulator_V1")
{
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
}

void CameraV1::stopExposure()
{
}

void CameraV1::abortExposure()
{
}

void CameraV1::pulseGuide(Alpaca::Camera::PulseGuideDirection direction, int duration)
{
}

void CameraV1::generateImage()
{

}

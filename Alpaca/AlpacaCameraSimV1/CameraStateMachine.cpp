#include "stdafx.h"
#include "CameraStateMachine.h"

CameraStateMachine::CameraStateMachine(std::shared_ptr<CameraV1> pCam)
	: CSMBase(0, "Alpaca Cam V1 SM", pCam)
{
}

CameraStateMachine::~CameraStateMachine()
{
}


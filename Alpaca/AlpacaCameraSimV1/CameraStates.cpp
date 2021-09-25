#include "stdafx.h"
#include "CameraV1.h"
#include "CameraStates.h"
#include "CamEvents.h"

using namespace Camera;
using namespace Camera::Event;
using namespace Camera::States;

void Camera::States::CameraState::addTransition(Event::EventID eventID, StateID targetStateID)
{
	CSMState::addTransition(static_cast<uint32_t>(eventID), static_cast<uint32_t>(targetStateID));
}

bool Camera::States::CameraState_Unknown::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Unknown::ticState(TSMContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Unknown::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Unknown::initTransitionTable()
{
}

bool Camera::States::CameraState_NotConnected::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	return true;
}

bool Camera::States::CameraState_NotConnected::ticState(TSMContextPtr pCtx)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	pCam->onStart();
	return true;
}

bool Camera::States::CameraState_NotConnected::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_NotConnected::initTransitionTable()
{
	addTransition(EventID::CamEvent_Connect, States::CameraState::StateID::CamState_Connecting);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Connecting::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	pCam->setupCamera();

	return true;
}

bool Camera::States::CameraState_Connecting::ticState(TSMContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Connecting::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Connecting::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Idle::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	return true;
}

bool Camera::States::CameraState_Idle::ticState(TSMContextPtr pCtx)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	pCam->onIdle();
	return true;
}

bool Camera::States::CameraState_Idle::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Idle::initTransitionTable()
{
	addTransition(EventID::CamEvent_TransferImage,  States::CameraState::StateID::CamState_Transferring);
	addTransition(EventID::CamEvent_StartExposure,	States::CameraState::StateID::CamState_Exposing);
	addTransition(EventID::CamEvent_PulseGuide,		States::CameraState::StateID::CamState_PulseGuiding);
	addTransition(EventID::CamEvent_Disconnect,     States::CameraState::StateID::CamState_Disconnecting);
	addTransition(EventID::CamEvent_Unknown,	    States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam  = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	m_pContext = std::make_shared<Camera::ExposureManager>(pCam);

	auto stateFirst = static_cast<unsigned int>(CameraState::StateID::CamState_Exposing_FirstSubstate);
	auto stateLast = static_cast<unsigned int>(CameraState::StateID::CamState_Exposing_LastSubstate);

	TStateList stateTable;

	// init our state list and transition tables
	for (auto i = stateFirst; i <= stateLast; i++)
	{
		auto id = static_cast<CameraState::StateID>(i);
		auto pState = createState(id);

		if (pState)
		{
			pState->initTransitionTable();
			stateTable.insert_or_assign(i, pState);
		}
	}

	setStateTable(stateTable);

	auto currentState = static_cast<uint32_t>(StateID::CamState_Exposing_GeneratingBackground);
	auto errorState   = static_cast<uint32_t>(StateID::CamState_Exposing_Stopped);
	// remember of offset using the position of the atual first 

	auto pErrorEvent = Event::createErrorEvent();

	setErrorEvent(pErrorEvent);
	setCurrentState(stateTable[currentState]);
	setErrorState(stateTable[errorState]);

	return true;
}

bool Camera::States::CameraState_Exposing::ticState(TSMContextPtr pCtx)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	onTic();
	auto pExpManager = std::dynamic_pointer_cast<Camera::ExposureManager>(m_pContext);
	if (pExpManager->getIsComplete())
	{
		pCam->finishExposure();
	}
	return true;
}

bool Camera::States::CameraState_Exposing::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success,       States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_AbortExposure, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure,       States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown,	   States::CameraState::StateID::CamState_Unknown);
}


bool Camera::States::CameraState_Transferring::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	if (!pCam)
	{
		return false;
	}
	return true;
}

bool Camera::States::CameraState_Transferring::ticState(TSMContextPtr pCtx)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	if (!pCam)
	{
		return false;
	}
	pCam->transferImage();

	return true;
}

bool Camera::States::CameraState_Transferring::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	if (!pCam)
	{
		return false;
	}
	pCam->readChunk();

	return true;
}

void Camera::States::CameraState_Transferring::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Error::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	pCam->setCameraState(Alpaca::Camera::CameraStateID::CameraError);
	pCam->recoverState();
	return true;
}

bool Camera::States::CameraState_Error::ticState(TSMContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Error::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Error::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}

	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::ticState(TSMContextPtr pCtx)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}
	pExpContext->onGenerateBackground();
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}
	return true;
}

void Camera::States::CameraState_Exposing_GeneratingBackground::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Exposing_GeneratingImage);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing_GeneratingImage::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}

	pExpContext->onGenerateImg();
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingImage::ticState(TSMContextPtr pCtx)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingImage::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pExpContext = std::dynamic_pointer_cast<Camera::ExposureManager>(pCtx);
	if (!pExpContext)
	{
		return false;
	}
	return true;
}

void Camera::States::CameraState_Exposing_GeneratingImage::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Exposing_Stopped);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Exposing_Stopped);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Exposing_Stopped);
}



bool Camera::States::CameraState_Exposing_Stopped::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Stopped::ticState(TSMContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Stopped::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing_Stopped::initTransitionTable()
{
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_PulseGuiding::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;;
}

bool Camera::States::CameraState_PulseGuiding::ticState(TSMContextPtr pCtx)
{
	return true;;
}

bool Camera::States::CameraState_PulseGuiding::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;;
}

void Camera::States::CameraState_PulseGuiding::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Disconnecting);
}

bool Camera::States::CameraState_Disconnecting::enterState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	pCam->shutdownCamera();
	return true;
}

bool Camera::States::CameraState_Disconnecting::ticState(TSMContextPtr pCtx)
{
	return true;;
}

bool Camera::States::CameraState_Disconnecting::exitState(TSMContextPtr pCtx, TEventPtr pEvent)
{
	return true;;
}

void Camera::States::CameraState_Disconnecting::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_NotConnected);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Error);
}


CamStatePtr Camera::States::createState(CameraState::StateID id)
{
	CamStatePtr pState = nullptr;

	switch (id)
	{
	case Camera::States::CameraState::StateID::CamState_Unknown:
		pState = std::make_shared<CameraState_Unknown>();
		break;
	case Camera::States::CameraState::StateID::CamState_NotConnected:
		pState = std::make_shared<CameraState_NotConnected>();
		break;
	case Camera::States::CameraState::StateID::CamState_Connecting:
		pState = std::make_shared<CameraState_Connecting>();
		break;
	case Camera::States::CameraState::StateID::CamState_Idle:
		pState = std::make_shared<CameraState_Idle>();
		break;
	case Camera::States::CameraState::StateID::CamState_Exposing:
		pState = std::make_shared<CameraState_Exposing>();
		break;
	case Camera::States::CameraState::StateID::CamState_Exposing_GeneratingBackground:
		pState = std::make_shared<CameraState_Exposing_GeneratingBackground>();
		break;
	case Camera::States::CameraState::StateID::CamState_Exposing_GeneratingImage:
		pState = std::make_shared<CameraState_Exposing_GeneratingImage>();
		break;
	case Camera::States::CameraState::StateID::CamState_Transferring:
		pState = std::make_shared<CameraState_Transferring>();
		break;
	case Camera::States::CameraState::StateID::CamState_Exposing_Stopped:
		pState = std::make_shared<CameraState_Exposing_Stopped>();
		break;
	case Camera::States::CameraState::StateID::CamState_PulseGuiding:
		pState = std::make_shared<CameraState_PulseGuiding>();
		break;
	case Camera::States::CameraState::StateID::CamState_Disconnecting:
		pState = std::make_shared<CameraState_Disconnecting>();
		break;
	case Camera::States::CameraState::StateID::CamState_Error:
		pState = std::make_shared<CameraState_Error>();
		break;
	default:
		break;
	}

	return pState;
}
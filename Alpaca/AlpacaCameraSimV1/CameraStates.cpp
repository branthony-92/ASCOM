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

bool Camera::States::CameraState_Unknown::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Unknown::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Unknown::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Unknown::initTransitionTable()
{
}

bool Camera::States::CameraState_NotConnected::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	pCam->onStart();
	return true;
}

bool Camera::States::CameraState_NotConnected::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_NotConnected::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_NotConnected::initTransitionTable()
{
	addTransition(EventID::CamEvent_Connect, States::CameraState::StateID::CamState_Connecting);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Connecting::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	pCam->setupCamera();

	return true;
}

bool Camera::States::CameraState_Connecting::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Connecting::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Connecting::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Idle::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	pCam->onIdle();
	return true;
}

bool Camera::States::CameraState_Idle::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Idle::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Idle::initTransitionTable()
{
	addTransition(EventID::CamEvent_StartExposure,	States::CameraState::StateID::CamState_Exposing);
	addTransition(EventID::CamEvent_PulseGuide,		States::CameraState::StateID::CamState_PulseGuiding);
	addTransition(EventID::CamEvent_Disconnect,     States::CameraState::StateID::CamState_Disconnecting);
	addTransition(EventID::CamEvent_Unknown,	    States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing::enterState(TContextPtr pCtx, TEventPtr pEvent)
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
			stateTable.push_back(pState);
		}
	}

	setStateTable(stateTable);

	StateID currentState = pCam->getGenerateBackground() ? StateID::CamState_Exposing_GeneratingBackground :
														   StateID::CamState_Exposing_GeneratingImage;

	// remember of offset using the position of the atual first 

	auto pErrorEvent = Event::createErrorEvent();

	setErrorEvent(pErrorEvent);
	setCurrentState(createState(currentState));
	setErrorState(createState(StateID::CamState_Exposing_Stopped));

	onTic();
	return true;
}

bool Camera::States::CameraState_Exposing::ticState(TContextPtr pCtx)
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

bool Camera::States::CameraState_Exposing::exitState(TContextPtr pCtx, TEventPtr pEvent)
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

bool Camera::States::CameraState_Error::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);
	pCam->setCameraState(Alpaca::Camera::CameraStateID::CameraError);
	pCam->recoverState();
	return true;
}

bool Camera::States::CameraState_Error::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Error::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Error::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingBackground::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing_GeneratingBackground::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Exposing_GeneratingImage);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_Exposing_GeneratingImage::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingImage::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Exposing_GeneratingImage::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing_GeneratingImage::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Exposing_Transferring);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Exposing_Stopped);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Exposing_Stopped);
}

bool Camera::States::CameraState_Exposing_Transferring::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Transferring::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Transferring::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing_Transferring::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Exposing_Stopped);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Exposing_Stopped);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Exposing_Stopped);
}


bool Camera::States::CameraState_Exposing_Stopped::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Stopped::ticState(TContextPtr pCtx)
{
	return true;
}

bool Camera::States::CameraState_Exposing_Stopped::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void Camera::States::CameraState_Exposing_Stopped::initTransitionTable()
{
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Unknown);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Unknown);
}

bool Camera::States::CameraState_PulseGuiding::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;;
}

bool Camera::States::CameraState_PulseGuiding::ticState(TContextPtr pCtx)
{
	return true;;
}

bool Camera::States::CameraState_PulseGuiding::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;;
}

void Camera::States::CameraState_PulseGuiding::initTransitionTable()
{
	addTransition(EventID::CamEvent_Success, States::CameraState::StateID::CamState_Idle);
	addTransition(EventID::CamEvent_Failure, States::CameraState::StateID::CamState_Error);
	addTransition(EventID::CamEvent_Unknown, States::CameraState::StateID::CamState_Disconnecting);
}

bool Camera::States::CameraState_Disconnecting::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pCam = std::dynamic_pointer_cast<Camera::CameraV1>(pCtx);

	pCam->shutdownCamera();
	return true;
}

bool Camera::States::CameraState_Disconnecting::ticState(TContextPtr pCtx)
{
	return true;;
}

bool Camera::States::CameraState_Disconnecting::exitState(TContextPtr pCtx, TEventPtr pEvent)
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
	case Camera::States::CameraState::StateID::CamState_Exposing_Transferring:
		pState = std::make_shared<CameraState_Exposing_Transferring>();
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
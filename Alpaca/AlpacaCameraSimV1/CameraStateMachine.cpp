#include "stdafx.h"
#include "CameraStateMachine.h"
#include "CameraStates.h"
#include "CamEvents.h"

using namespace SM;
using namespace Camera;
using namespace States;

CameraStateMachine::CameraStateMachine(std::shared_ptr<CameraV1> pCam)
	: CSMBase(0, "Alpaca Cam V1 SM", pCam)
	, m_pCamera(pCam)
{
	initSM();
}

CameraStateMachine::~CameraStateMachine()
{
}

void SM::CameraStateMachine::initSM()
{
	auto stateFirst = static_cast<unsigned int>(CameraState::StateID::CamState_FirstState);
	auto stateLast  = static_cast<unsigned int>(CameraState::StateID::NumBaseStates);

	TStateList stateTable;

	// init our state list and transition tables
	for (auto i = stateFirst; i < stateLast; i++)
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

	auto firstState = static_cast<unsigned int>(CameraState::StateID::CamState_NotConnected);
	auto errorState = static_cast<unsigned int>(CameraState::StateID::CamState_Error);
	auto pErrorEvent = Event::createErrorEvent();

	setErrorEvent(pErrorEvent);
	setErrorState(m_stateTable[errorState]);

	startSM(m_stateTable[firstState]);
}


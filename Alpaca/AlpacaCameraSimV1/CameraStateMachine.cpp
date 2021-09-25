#include "stdafx.h"
#include "CameraStateMachine.h"
#include "CameraStates.h"
#include "CamEvents.h"

using namespace SM;
using namespace Camera;
using namespace States;

DeviceStateMachine::DeviceStateMachine(TSMContextPtr pDev, std::string name)
	: CSMBase(0, name, pDev)
{
}

DeviceStateMachine::~DeviceStateMachine()
{
}

void SM::DeviceStateMachine::initSM()
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
	setCurrentState(m_stateTable[firstState]);
	m_smEnabled = true;
}


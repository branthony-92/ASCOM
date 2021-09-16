#include "stdafx.h"
#include "CameraSimApp.h"
#include "qcommandlineparser.h"
#include <fstream>>

using namespace App;

App::CameraSimApp::CameraSimApp(int argc, char* argv[])
	: QApplication(argc, argv)
	, m_pServer(nullptr)
	, m_pCameraStateMachine(nullptr)
	, m_pMainFrame(nullptr)
{
}

App::CameraSimApp::~CameraSimApp()
{
	if (arguments().contains("-dumpstates"))
	{
		auto states = m_pCameraStateMachine->dumpStateHistory();

		std::ofstream file;
		file.open("StateHistory.csv", std::fstream::trunc | std::fstream::out);
		if (!file.is_open()) return;

		file << states;
		
		file.close();
	}
	m_pCameraStateMachine = nullptr;
	m_pMainFrame = nullptr;
	m_pServer = nullptr;
}

bool CameraSimApp::init()
{
	m_pServer = std::make_shared<CamServer::CameraServer>();

	// create and devices and register them as server contexts
	auto pCam = std::make_shared<Camera::CameraV1>(0);
	m_pServer->registerContext(pCam);

	// init the state machine for the camera
	m_pMainFrame = std::make_shared<Gui::DlgMainFrame>(m_pServer);
	m_pMainFrame->show();

	m_pCameraStateMachine = std::make_shared<SM::CameraStateMachine>(pCam);

	// check for command line args here
	auto args = arguments();
	m_pCameraStateMachine->enableStateHistory(args.contains("-dumpstates"));

	return true;
}

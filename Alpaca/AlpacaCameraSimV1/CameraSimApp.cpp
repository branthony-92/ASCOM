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
	auto pCam = std::make_shared<Camera::CameraV1>(0);
	auto pServer = std::make_shared<CamServer::CameraServer>(pCam);

	// init the state machine for the camera

	auto pMF = std::make_shared<Gui::DlgMainFrame>(pServer);
	pMF->show();

	auto pCamStateMachine = std::make_shared<SM::CameraStateMachine>(pCam);

	auto args = arguments();

	pCamStateMachine->enableStateHistory(args.contains("-dumpstates"));

	m_pServer = pServer;
	m_pCameraStateMachine = pCamStateMachine;
	m_pMainFrame = pMF;

	return true;
}

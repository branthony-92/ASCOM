#ifndef CAMSIMAPP_H
#define CAMSIMAPP_H

#include <QtCore/QCoreApplication>

#include "DlgMainFrame.h"
#include "CamServer.h"
#include "CameraV1.h"
#include "CameraStateMachine.h"

namespace App {

class CameraSimApp : public QApplication
{
	CamServer::CamServerPtr   m_pServer;
	SM::CameraSMPtr           m_pCameraStateMachine;
	Gui::AppGuiPtr            m_pMainFrame;

public:

	CameraSimApp(int argc, char* argv[]);
	virtual ~CameraSimApp();
	bool init();
};

}
#endif
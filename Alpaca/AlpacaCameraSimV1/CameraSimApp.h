#ifndef CAMSIMAPP_H
#define CAMSIMAPP_H

#include <QApplication>
#include <qcommandlineparser.h>

#include "DlgMainFrame.h"
#include "CamServer.h"
#include "CameraV1.h"
#include "CameraStateMachine.h"

class QCommandLineParser;

namespace App {

class CameraSimApp : public QApplication
{
	CamServer::CamServerPtr   m_pServer;
	SM::DeviceSMManagerPtr    m_pDeviceSMMaster;
	Gui::AppGuiPtr            m_pMainFrame;
	QCommandLineParser        m_parser;

public:

	CameraSimApp(int argc, char* argv[]);
	virtual ~CameraSimApp();
	bool init();
	bool initWithoutGui();

	ServerConfig loadConfigFromFile(std::string file);
};

}
#endif
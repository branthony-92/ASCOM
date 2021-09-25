#ifndef CAMSIMAPP_H
#define CAMSIMAPP_H

#include <QApplication>
#include <qcommandlineparser.h>

#include "DlgMainFrame.h"
#include "DeviceManager.h"

class QCommandLineParser;

namespace App {

class CameraSimApp : public QApplication
{
	DevServer::DeviceServerPtr   m_pServer;
	DeviceManagerPtr             m_pDeviceManager;
	Gui::AppGuiPtr               m_pMainFrame;
	QCommandLineParser           m_parser;

public:

	CameraSimApp(int argc, char* argv[]);
	virtual ~CameraSimApp();
	bool init();
	bool initWithoutGui();
	ServerConfig loadConfigFromFile(std::string file);
};

}
#endif
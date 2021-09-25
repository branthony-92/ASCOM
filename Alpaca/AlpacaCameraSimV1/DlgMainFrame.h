#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmap.h>
#include <qscopedpointer.h>
#include <qlabel.h>
#include <qtextedit.h>
#include "ui_DlgMainFrame.h"

#include "CameraV1.h"
#include "CamServer.h"
#include "DeviceManager.h"
#include "DeviceView.h"


namespace Gui {

class DlgMainFrame 
	: public QMainWindow
	, public View::DeviceView
{
	Q_OBJECT

	// UI properties
	DEVICEPROP(DevServer::DeviceServer::ServerConfigurationData, ConfigData, configData)
	DEVICEPROP(QString, CertPathHistory,             certPathHistory)
	DEVICEPROP(QString, PrivKeyPathHistory,          privKeyPathHistory)
	DEVICEPROP(QString, EncryptionParamsPathHistory, encryptionParamsPathHistory)
	DEVICEPROP(QString, StatusString,				 statusString)
	DEVICEPROP(DevServer::DeviceServerPtr, DevServer, pDevServer)
	DEVICEPROP(DeviceManagerPtr,        pDeviceManager, pDeviceManager)

public:
	DlgMainFrame(DeviceManagerPtr pDevManager, DevServer::DeviceServerPtr pServer, QWidget *parent = Q_NULLPTR);
	~DlgMainFrame();

	void update(const Hint::HintBase&) override;


private:
	Ui::DlgMainFrame ui;

	using QLabelPtr    = std::shared_ptr<QLabel>;
	using QTextEditPtr = std::shared_ptr<QTextEdit>;

	struct DeviceWidget 
	{
		QLabelPtr    pLabel;
		QTextEditPtr pEdit;
	};
	std::map<unsigned int, DeviceWidget> m_deviceStatusMap;
	

	void initSlots();
	void loadSettings();
	void saveSettings();
	void updateValues(bool dataIn = true);

	void enableButtonControls(bool enabled);

public slots:

	void onStartServer();
	void onStopServer();


	void onActionFile_Close();
	void onActionServer_Settings();

	void onActionDevice_ManagerDevices();

	void onUpdateCamStatus(unsigned int name, QString msg);
	void onUpdateServerStatus(QString msg);
	void onUpdateDeviceRegistered(unsigned int id, QString name);

signals:
	void CamStatusUpdated(unsigned int, QString);
	void ServerStatusUpdated(QString);
	void DeviceRegistered(unsigned int id, QString name);
};
typedef std::shared_ptr<DlgMainFrame> AppGuiPtr;
}
#endif // !MAINWINDOW_H
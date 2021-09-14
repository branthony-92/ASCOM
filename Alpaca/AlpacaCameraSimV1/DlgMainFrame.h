#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_DlgMainFrame.h"

#include "CameraV1.h"
#include "CamServer.h"
#include "DeviceView.h"
namespace Gui {

class DlgMainFrame 
	: public QMainWindow
	, public View::DeviceView
{
	Q_OBJECT

	// UI properties
	DEVICEPROP(CamServer::CameraServer::ServerConfigurationData, ConfigData, configData)
	DEVICEPROP(QString, CertPathHistory,             certPathHistory)
	DEVICEPROP(QString, PrivKeyPathHistory,          privKeyPathHistory)
	DEVICEPROP(QString, EncryptionParamsPathHistory, encryptionParamsPathHistory)
	DEVICEPROP(QString, StatusString,				 statusString)
	DEVICEPROP(CamServer::CamServerPtr, CamServer, pCamServer)

public:
	DlgMainFrame(CamServer::CamServerPtr pServer, QWidget *parent = Q_NULLPTR);
	~DlgMainFrame();

	void update(const Hint::HintBase&) override;


private:
	Ui::DlgMainFrame ui;

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

	void onUpdateCamStatus(QString msg);
	void onUpdateServerStatus(QString msg);
signals:
	void CamStatusUpdated(QString);
	void ServerStatusUpdated(QString);
};
typedef std::shared_ptr<DlgMainFrame> AppGuiPtr;
}
#endif // !MAINWINDOW_H
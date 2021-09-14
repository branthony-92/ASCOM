#ifndef DLGSERVERSETUP_H
#define DLGSERVERSETUP_H

#include <QDialog>
#include "ui_DlgServerSetup.h"
#include "CamServer.h"

class DlgServerSetup : public QDialog
{
	Q_OBJECT

public:
	DlgServerSetup(CamServer::CameraServer::ServerConfigurationData config, QWidget *parent = Q_NULLPTR);
	~DlgServerSetup();

	DEVICEPROP(CamServer::CameraServer::ServerConfigurationData, ConfigData, configData)
	DEVICEPROP(QString, CertPathHistory, certPathHistory)
	DEVICEPROP(QString, PrivKeyPathHistory, privKeyPathHistory)
	DEVICEPROP(QString, EncryptionParamsPathHistory, encryptionParamsPathHistory)
	DEVICEPROP(int, WindowPosX, windowPosX)
	DEVICEPROP(int, WindowPosY, windowPosY)


private:
	Ui::DlgServerSetup ui;

	void initSlots();

	void updateValues(bool dataIn = true);

	void enableHTTPControls(bool enabed);
	void enableButtonControls(bool enabled);

	void loadPosition();
	void savePosition();

public slots:

	void onHTTPSEnabledToggled(bool checked);
	void onCertExplorerClicked();
	void onPrivKeyExplorerClicked();
	void onEncyptionParamsExplorerClicked();

	void onOKClicked();

	void closeEvent(QCloseEvent*) override;
	void showEvent(QShowEvent*) override;
};

#endif
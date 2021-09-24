#include "stdafx.h"
#include "DlgMainFrame.h"

#include "DlgServerSetup.h"

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qaction>

using namespace Gui;
using namespace CamServer;

DlgMainFrame::DlgMainFrame(CamServerPtr pServer, QWidget *parent)
	: QMainWindow(parent)
	, m_pCamServer(pServer)
	, m_certPathHistory("")
	, m_privKeyPathHistory("")
	, m_encryptionParamsPathHistory("")
	, m_statusString("")
	, m_deviceStatusMap()
{
	ui.setupUi(this);

	for (auto& pCtx : pServer->getContexts())
	{
		auto pModel = std::dynamic_pointer_cast<Model::DeviceModel>(pCtx);
		if (pModel)
		{
			pModel->registerView(this);

			auto pDevice = std::dynamic_pointer_cast<Alpaca::AlpacaDeviceV1>(pCtx);
			if (!pDevice) continue;

			onUpdateDeviceRegistered(pDevice->getDeviceID(), QString::fromStdString(pDevice->getName()));
		}
	}

	initSlots();

	loadSettings();
	updateValues(false);

	enableButtonControls(true);

	onUpdateServerStatus("Camera Server Not Listening");
}

DlgMainFrame::~DlgMainFrame()
{
	for (auto& pCtx : m_pCamServer->getContexts())
	{
		auto pModel = std::dynamic_pointer_cast<Model::DeviceModel>(pCtx);
		if (pModel)
		{
			pModel->unregisterView(this);
		}
	}
	saveSettings();
}

void Gui::DlgMainFrame::update(const Hint::HintBase& hint)
{
	using namespace Hint;
	switch (hint.c_hintID)
	{
	case HintBase::HintID::UpdateCameraStatus:
	{
		auto pHint = dynamic_cast<const UpdateCamStatusHint*>(&hint);
		if (pHint)
		{
			emit CamStatusUpdated(pHint->m_Data.first, QString::fromStdString(pHint->m_Data.second));
		}
		break;
	}
	case HintBase::HintID::UpdateServerStatus:
	{
		auto pHint = dynamic_cast<const UpdateServertatusHint*>(&hint);
		if (pHint)
		{
			emit ServerStatusUpdated(QString::fromStdString(pHint->m_Data));
		}
		break;
	}
	case HintBase::HintID::RegisterDeviceToGui:
	{
		auto pHint = dynamic_cast<const RegisterDeviceToGuiHint*>(&hint);
		if (pHint)
		{
			emit DeviceRegistered(pHint->m_Data.first, QString::fromStdString(pHint->m_Data.second));
		}
		break;
	}
	default:
		break;
	}
}


void DlgMainFrame::enableButtonControls(bool enabled)
{
	ui.StartServerButton->setEnabled(enabled);
	ui.StopServerButton->setEnabled(!enabled);

	ui.actionSettings->setEnabled(enabled);
}

void DlgMainFrame::initSlots()
{
	connect(ui.actionClose,
		&QAction::triggered, this,
		&DlgMainFrame::onActionFile_Close);

	connect(ui.actionSettings,
		&QAction::triggered,this, 
		&DlgMainFrame::onActionServer_Settings);

	connect(ui.StartServerButton,
		&QPushButton::clicked, this,
		&DlgMainFrame::onStartServer);

	connect(ui.StopServerButton,
		&QPushButton::clicked, this,
		&DlgMainFrame::onStopServer);

	connect(this, SIGNAL(CamStatusUpdated(unsigned int,QString)),          this, SLOT(onUpdateCamStatus(unsigned int,QString)));
	connect(this, SIGNAL(ServerStatusUpdated(QString)),                    this, SLOT(onUpdateServerStatus(QString)));
	connect(this, SIGNAL(DeviceRegistered(unsigned int, QString)),         this, SLOT(onUpdateDeviceRegistered(unsigned int,QString)));
}

void DlgMainFrame::loadSettings()
{
	QSettings settings("Alpaca Cam Sim V1");
	settings.beginGroup("Server Config");
	m_configData.configFilePath             = settings.value("Config_Path", "").toString().toStdString();
	m_configData.certificateFilePath        = settings.value("Cert_Path", "").toString().toStdString();
	m_configData.privKeyFilePath            = settings.value("Key_Path", "").toString().toStdString();
	m_configData.diffieHellmanParamFilePath = settings.value("Encryption_Param_Path", "").toString().toStdString();
	m_configData.IPv4Address                = settings.value("IPv4Address", "127.0.0.1").toString().toStdString();
	m_configData.port						= settings.value("Port", 8080).toInt();
	m_configData.serverThreadCount			= settings.value("Listener_Threads", 1).toInt();
	m_configData.useHTTPS					= settings.value("UseHTTPS", false).toBool();

	m_certPathHistory                = settings.value("PathHistory_Cert", "").toString();
	m_privKeyPathHistory             = settings.value("PathHistory_PrivKey", "").toString();
	m_encryptionParamsPathHistory	 = settings.value("PathHistory_EncryptionParam", "").toString();

	settings.endGroup();
}

void DlgMainFrame::saveSettings()
{
	QSettings settings("Alpaca Cam Sim V1");
	settings.beginGroup("Server Config");

	settings.setValue("Config_Path",           QString::fromStdString(m_configData.configFilePath));
	settings.setValue("Cert_Path",             QString::fromStdString(m_configData.certificateFilePath));
	settings.setValue("Key_Path",              QString::fromStdString(m_configData.privKeyFilePath));
	settings.setValue("Encryption_Param_Path", QString::fromStdString(m_configData.diffieHellmanParamFilePath));
	settings.setValue("IPv4Address",		   QString::fromStdString(m_configData.IPv4Address));
	settings.setValue("Port",				   m_configData.port);
	settings.setValue("Listener_Threads",      m_configData.serverThreadCount);
	settings.setValue("UseHTTPS",			   m_configData.useHTTPS);

	settings.value("PathHistory_Cert",            m_certPathHistory);
	settings.value("PathHistory_PrivKey",         m_privKeyPathHistory);
	settings.value("PathHistory_EncryptionParam", m_encryptionParamsPathHistory);

	settings.endGroup();
}

void DlgMainFrame::updateValues(bool dataIn)
{
	if (dataIn)
	{

	}
	else
	{
	}
}


void DlgMainFrame::onStartServer()
{
	updateValues();

	if (!m_pCamServer->start(m_configData))
	{
		auto errMsg = m_pCamServer->getLastError();
		QMessageBox msgBox;
		msgBox.setText("Could Not Start Camera Server: " + QString::fromStdString(errMsg));
		msgBox.exec();
		return;
	}

	QString schema = m_configData.useHTTPS ? "https" : "http";
	QString port = QString("%1").arg(m_configData.port);
	QString ipAddress = QString::fromStdString(m_configData.IPv4Address);

	QString message = "Camera Server Listening on " + schema + "://" + ipAddress + ":" + port;

	onUpdateServerStatus(message);
	enableButtonControls(false);
	saveSettings();
}

void DlgMainFrame::onStopServer()
{
	m_pCamServer->stop();
	onUpdateServerStatus("Camera Server Not Listening");
	enableButtonControls(true);
}

void DlgMainFrame::onUpdateCamStatus(unsigned int name, QString msg)
{
	m_statusString = msg;
	if (m_deviceStatusMap.find(name) != m_deviceStatusMap.end())
	{
		auto data = m_deviceStatusMap.at(name);
		data.pEdit->setText(msg);
	}
	//ui.CameraStatusEdit->setPlainText(msg);
}

void Gui::DlgMainFrame::onUpdateServerStatus(QString msg)
{
	m_statusString = msg;
	ui.ServerStatusEdit->setPlainText(msg);
}

void Gui::DlgMainFrame::onUpdateDeviceRegistered(unsigned int id, QString name)
{
	// We don't want to add another entry with the same name
	if (m_deviceStatusMap.find(id) == m_deviceStatusMap.end())
	{
		// create the structure that will hold our status lable and edit
		DeviceWidget deviceStatus;
		deviceStatus.pLabel = std::make_shared<QLabel>(this);
		deviceStatus.pEdit  = std::make_shared<QTextEdit>(this);

		// add it to the map 
		m_deviceStatusMap.insert_or_assign(id, deviceStatus);

		ui.DeviceStatus->layout()->addWidget(deviceStatus.pLabel.get());
		ui.DeviceStatus->layout()->addWidget(deviceStatus.pEdit.get());

		auto prettyName = name.replace('_', ' ');
		deviceStatus.pLabel->setText(prettyName);
		deviceStatus.pLabel->show();
		deviceStatus.pEdit->show();
		deviceStatus.pEdit->setReadOnly(true);
	}
}

void DlgMainFrame::onActionFile_Close()
{
	saveSettings();
	close();
}

void DlgMainFrame::onActionServer_Settings()
{
	DlgServerSetup dlg(m_configData, this);

	auto ret = dlg.exec();
	if (ret)
	{
		m_configData = dlg.getConfigData();
	}
}


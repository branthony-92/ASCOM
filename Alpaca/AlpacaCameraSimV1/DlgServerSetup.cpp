#include "stdafx.h"
#include "DlgServerSetup.h"
#include <qsettings.h>
#include <qfiledialog.h>

DlgServerSetup::DlgServerSetup(CamServer::CameraServer::ServerConfigurationData config, QWidget* parent)
	: QDialog(parent)
	, m_configData(config)
	, m_windowPosX(0)
	, m_windowPosY(0)
{
	ui.setupUi(this);
	initSlots();
	updateValues(false);
	loadPosition();
}

DlgServerSetup::~DlgServerSetup()
{
}


void DlgServerSetup::onHTTPSEnabledToggled(bool checked)
{
	m_configData.useHTTPS = checked;
	enableHTTPControls(checked);
}

void DlgServerSetup::enableHTTPControls(bool enabed)
{
	ui.CertOpenExplorerButton->setEnabled(enabed);
	ui.CertFilePath->setEnabled(enabed);

	ui.PrivKeyOpenExplorerButton->setEnabled(enabed);
	ui.PrivKeyFilePath->setEnabled(enabed);

	ui.EncryptionParamsOpenFileExplorerButton->setEnabled(enabed);
	ui.EncryptionParamFilePath->setEnabled(enabed);
}

void DlgServerSetup::enableButtonControls(bool enabled)
{
	ui.IPAddressEdit->setEnabled(enabled);
	ui.PortEdit->setEnabled(enabled);
	ui.ThreadCountSpin->setEnabled(enabled);

	if (m_configData.useHTTPS)
	{
		ui.CertOpenExplorerButton->setEnabled(enabled);
		ui.PrivKeyOpenExplorerButton->setEnabled(enabled);
		ui.EncryptionParamsOpenFileExplorerButton->setEnabled(enabled);

		ui.CertFilePath->setEnabled(enabled);
		ui.PrivKeyFilePath->setEnabled(enabled);
		ui.EncryptionParamFilePath->setEnabled(enabled);
		ui.EnableHTTPSCheck->setEnabled(enabled);
	}
}

void DlgServerSetup::loadPosition()
{
	QSettings settings("Alpaca Cam Sim V1");
	settings.beginGroup("Server Config Settings");

	m_windowPosX = settings.value("PosX", 0).toInt();
	m_windowPosY = settings.value("PosX", 0).toInt();

	settings.endGroup();
}

void DlgServerSetup::savePosition()
{
	QSettings settings("Alpaca Cam Sim V1");
	settings.beginGroup("Server Config Settings");

	settings.setValue("PosX", pos().x());
	settings.setValue("PosX", pos().y());

	settings.endGroup();
}


void DlgServerSetup::initSlots()
{
	connect(ui.EnableHTTPSCheck,
		&QCheckBox::toggled, this,
		&DlgServerSetup::onHTTPSEnabledToggled);

	connect(ui.CertOpenExplorerButton,
		&QPushButton::clicked, this,
		&DlgServerSetup::onCertExplorerClicked);

	connect(ui.PrivKeyOpenExplorerButton,
		&QPushButton::clicked, this,
		&DlgServerSetup::onPrivKeyExplorerClicked);

	connect(ui.EncryptionParamsOpenFileExplorerButton,
		&QPushButton::clicked, this,
		&DlgServerSetup::onEncyptionParamsExplorerClicked);

	connect(ui.OKButton,
		&QPushButton::clicked, this,
		&DlgServerSetup::onOKClicked);
}


void DlgServerSetup::updateValues(bool dataIn)
{
	if (dataIn)
	{
		bool ok = false;
		m_configData.port = ui.PortEdit->toPlainText().toUInt(&ok);
		m_configData.IPv4Address = ui.IPAddressEdit->toPlainText().toStdString();
		m_configData.serverThreadCount = ui.ThreadCountSpin->value();


		m_configData.useHTTPS = ui.EnableHTTPSCheck->isChecked();
		m_configData.certificateFilePath = ui.CertFilePath->toPlainText().toStdString();
		m_configData.privKeyFilePath = ui.PrivKeyFilePath->toPlainText().toStdString();
		m_configData.diffieHellmanParamFilePath = ui.EncryptionParamFilePath->toPlainText().toStdString();
	}
	else
	{
		ui.PortEdit->setPlainText(QString("%1").arg(m_configData.port));
		ui.IPAddressEdit->setPlainText(QString::fromStdString(m_configData.IPv4Address));
		ui.ThreadCountSpin->setValue(m_configData.serverThreadCount);

		ui.CertFilePath->setPlainText(QString::fromStdString(m_configData.certificateFilePath));
		ui.PrivKeyFilePath->setPlainText(QString::fromStdString(m_configData.privKeyFilePath));
		ui.EncryptionParamFilePath->setPlainText(QString::fromStdString(m_configData.diffieHellmanParamFilePath));
		ui.EnableHTTPSCheck->setChecked(m_configData.useHTTPS);
		enableHTTPControls(m_configData.useHTTPS);
	}
}

void DlgServerSetup::onCertExplorerClicked()
{
	QFileDialog dlg(this,
		tr("Locate Certificate File"), m_certPathHistory, tr("Certificate Files (*.pem)"));

	if (dlg.exec() && !dlg.selectedFiles().empty())
	{
		auto fileName = dlg.selectedFiles().front();
		if (!fileName.isEmpty())
		{
			ui.CertFilePath->setText(fileName);
			m_configData.certificateFilePath = fileName.toStdString();
		}
		auto history = dlg.directory().absolutePath();
		m_certPathHistory = history;
	}
}

void DlgServerSetup::onPrivKeyExplorerClicked()
{
	QFileDialog dlg(this,
		tr("Locate Private Key File"), m_privKeyPathHistory, tr("Private Key Files (*.pem)"));

	if (dlg.exec() && !dlg.selectedFiles().empty())
	{
		auto fileName = dlg.selectedFiles().front();
		if (!fileName.isEmpty())
		{
			ui.PrivKeyFilePath->setText(fileName);
			m_configData.privKeyFilePath = fileName.toStdString();
		}
		auto history = dlg.directory().absolutePath();
		m_privKeyPathHistory = history;
	}
}

void DlgServerSetup::onEncyptionParamsExplorerClicked()
{
	QFileDialog dlg(this,
		tr("Locate Certificate File"), m_encryptionParamsPathHistory, tr("Encryption Param Files (*.pem)"));

	if (dlg.exec() && !dlg.selectedFiles().empty())
	{
		auto fileName = dlg.selectedFiles().front();
		if (!fileName.isEmpty())
		{
			ui.EncryptionParamFilePath->setText(fileName);
			m_configData.diffieHellmanParamFilePath = fileName.toStdString();
		}
		auto history = dlg.directory().absolutePath();
		m_encryptionParamsPathHistory = history;
	}
}

void DlgServerSetup::onOKClicked()
{
	updateValues();
	savePosition();
	accept();
}

void DlgServerSetup::closeEvent(QCloseEvent* pEvent)
{
	savePosition();
}

void DlgServerSetup::showEvent(QShowEvent* pEvent)
{

	//move(m_windowPosX, m_windowPosY);
}

#include "DlgDeviceManager.h"
#include "ui_DlgDeviceManager.h"

#include "DeviceManager.h"

#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>

const QStringList c_deviceTypes_Internal = {
	"camera",
	"covercalibrator",
	"dome",
	"filterwheel",
	"focuser",
	"observingcconditions",
	"rotator",
	"safetymonitor",
	"switch",
	"telescope"
};

const QStringList c_deviceTypes_Pretty = {
	"Camera",
	"Cover Calibrator",
	"Dome",
	"Filterwheel",
	"Focuser",
	"Observingc Conditions",
	"Rotator",
	"Safety Monitor",
	"Switch",
	"Telescope"
};


DlgDeviceManager::DlgDeviceManager(DeviceManager& devManager, QWidget *parent)
	: QDialog(parent)
	, m_deviceManager(devManager)
	, m_deviceIndexToID()
{
	ui = new Ui::DlgDeviceManager();
	ui->setupUi(this);

	initDeviceList();
	initTypeList();

	connect(ui->AddDevButton,
		&QPushButton::clicked, this,
		&DlgDeviceManager::onAddDevice);

	connect(ui->RemoveDevButton,
		&QPushButton::clicked, this,
		&DlgDeviceManager::onRemoveDevice);

}

DlgDeviceManager::~DlgDeviceManager()
{

	delete ui;
}

void DlgDeviceManager::initDeviceList()
{
	auto table = ui->DeviceTable;
	table->setColumnCount(3);
	table->horizontalHeader()->show();
	table->verticalHeader()->hide();
	table->setShowGrid(false);
	table->setContextMenuPolicy(Qt::CustomContextMenu);
	table->setMouseTracking(true);

	QStringList table1Labels;
	table1Labels << tr("Device Name") << tr("Device Type") << tr("Device Number");
	table->setHorizontalHeaderLabels(table1Labels);

	refreshList();
}

void DlgDeviceManager::initTypeList()
{
	for (auto& devType : c_deviceTypes_Pretty)
	{
		auto name = devType;
		ui->NewDevType->addItem(name);
		break; // so far we only support cameras
	}
	ui->NewDevType->setCurrentIndex(0);
}

void DlgDeviceManager::refreshList()
{
	auto table = ui->DeviceTable;
	table->clearContents();
	m_deviceIndexToID.clear();
	table->setRowCount(0);


	auto devices = m_deviceManager.getDeviceData();
	
	unsigned int index = 0u;
	for (auto& device : devices)
	{
		auto currIndex = index++;
		auto pDevice = std::dynamic_pointer_cast<Alpaca::AlpacaDeviceV1>(device.second->getContext());

		if (!pDevice) continue;

		DeviceData data;
		data.deviceName = pDevice->getName();
		data.deviceNum = pDevice->getDeviceNumber();
		data.deviceType = Alpaca::c_deviceTypeIDToStrings.at(pDevice->getDeviceType());

		auto name = QString::fromStdString(data.deviceName);
		auto type = QString::fromStdString(data.deviceType);
		auto num = QString("%1").arg(data.deviceNum);

		QTableWidgetItem* nameItem = new QTableWidgetItem(name);
		nameItem->setData(Qt::DisplayRole, QVariant(name));
		nameItem->setToolTip(name);
		nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
		nameItem->setTextAlignment((Qt::AlignCenter));

		QTableWidgetItem* typeItem = new QTableWidgetItem(type);
		typeItem->setData(Qt::UserRole + 1, QVariant(type));
		typeItem->setFlags(typeItem->flags() ^ Qt::ItemIsEditable);
		typeItem->setFlags(typeItem->flags() ^ Qt::ItemIsSelectable);
		typeItem->setTextAlignment((Qt::AlignCenter));

		QTableWidgetItem* deviceNumType = new QTableWidgetItem(num);
		deviceNumType->setData(Qt::UserRole + 1, QVariant(num));
		deviceNumType->setFlags(deviceNumType->flags() ^ Qt::ItemIsEditable);
		deviceNumType->setFlags(deviceNumType->flags() ^ Qt::ItemIsSelectable);
		deviceNumType->setTextAlignment((Qt::AlignCenter));

		auto rows = table->rowCount();
		table->insertRow(rows);
		table->setItem(rows, 0, nameItem);
		table->setItem(rows, 1, typeItem);
		table->setItem(rows, 2, deviceNumType);

		m_deviceIndexToID.insert_or_assign(currIndex, pDevice->getDeviceID());
	}
}

void DlgDeviceManager::onAddDevice()
{
	DeviceData data;
	data.deviceName = ui->NewDevName->toPlainText().toStdString();

	if (data.deviceName.empty())
	{
		QMessageBox msg;
		msg.setText("Device name cannot be blank");
		msg.exec();
		return;
	}


	auto index = ui->NewDevType->currentIndex();
	
	if (index > 0)
	{
		QMessageBox msg;
		msg.setText("Device  not supported yet");
		msg.exec();
		return;
	}
	
	data.deviceType = c_deviceTypes_Internal[index].toStdString();
	data.deviceNum = ui->NewDevNum->value();
	
	m_deviceManager.createDevice(data);
	refreshList();
}

void DlgDeviceManager::onRemoveDevice()
{
	auto table = ui->DeviceTable;

	auto index = table->currentRow();

	auto dev = m_deviceIndexToID.find(index);
	if (dev != m_deviceIndexToID.end())
	{
		m_deviceManager.removeDevice(dev->second);
		m_deviceIndexToID.erase(dev);
		refreshList();
	}
}
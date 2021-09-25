#pragma once

#include <QDialog>
#include <QList>

namespace Ui { class DlgDeviceManager; };
class DeviceManager;
class DlgDeviceManager : public QDialog
{
	Q_OBJECT

public:
	DlgDeviceManager(DeviceManager& devManager, QWidget *parent = Q_NULLPTR);
	~DlgDeviceManager();

public slots:
	void onAddDevice();
	void onRemoveDevice();

private:
	Ui::DlgDeviceManager *ui;
	DeviceManager& m_deviceManager;
	std::map<unsigned int, unsigned int> m_deviceIndexToID;

	void initDeviceList();
	void initTypeList();
	void refreshList();
};

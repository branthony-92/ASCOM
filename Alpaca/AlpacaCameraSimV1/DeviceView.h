#pragma once

namespace Hint {
	class HintBase
	{
	public:
		enum class HintID
		{
			UpdateCameraStatus,
			UpdateServerStatus,
			RegisterDeviceToGui,
		};

		HintBase(HintID id)
			: c_hintID(id)
		{}
		virtual ~HintBase() {}
		const HintID c_hintID;
	};

	template <class DataType, HintBase::HintID ID>
	class DeviceViewHint : public HintBase
	{
	public:
		DeviceViewHint(DataType& data)
			: HintBase(ID)
			, m_Data(data)
		{}
		virtual ~DeviceViewHint() {}
		const DataType& m_Data;
	};

	typedef std::pair<unsigned int, std::string> DeviceViewData;

	typedef DeviceViewHint<std::string, HintBase::HintID::UpdateServerStatus>	  UpdateServertatusHint;
	typedef DeviceViewHint<DeviceViewData, HintBase::HintID::UpdateCameraStatus>  UpdateCamStatusHint;
	typedef DeviceViewHint<DeviceViewData, HintBase::HintID::RegisterDeviceToGui>    RegisterDeviceToGuiHint;
}

namespace View {
	class DeviceView
	{
		uint32_t m_viewID;
		bool m_allowUpdates;
	public:
		DeviceView(bool allowUpdates = true) 
			: m_viewID(UINT32_MAX) 
			, m_allowUpdates(allowUpdates)
		{}

		uint32_t getID() const { return m_viewID; }
		void setID(uint32_t id) { m_viewID = id; }

		bool canUpdate() const { return m_allowUpdates; }
		virtual void update(const Hint::HintBase&) = 0;
	};

}


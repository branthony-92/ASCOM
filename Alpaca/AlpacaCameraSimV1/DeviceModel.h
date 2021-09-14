#pragma once

#include "DeviceView.h"

namespace Model {

	class DeviceModel
	{
		std::map<uint32_t, View::DeviceView*> m_views;
		uint32_t m_nextViewID;
	public:
		DeviceModel()
			: m_views()
			, m_nextViewID(0u)
		{}
		virtual ~DeviceModel()
		{
			m_views.clear();
		}

		void registerView(View::DeviceView* pView);
		void unregisterView(View::DeviceView* pView);
		void clearViews();

		void updateViews(const Hint::HintBase&);
	};

}

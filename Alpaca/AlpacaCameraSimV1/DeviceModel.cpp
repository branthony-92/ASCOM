#include "stdafx.h"
#include "DeviceModel.h"


void Model::DeviceModel::registerView(View::DeviceView* pView)
{
	auto view = m_views.find(m_nextViewID);
	if (view != m_views.end())
	{
		do
		{
			m_nextViewID++;
			view = m_views.find(m_nextViewID);
		} 		while (view != m_views.end());
	}
	pView->setID(m_nextViewID);
	m_views.insert_or_assign(m_nextViewID++, pView);
}

void Model::DeviceModel::unregisterView(View::DeviceView* pView)
{
	auto view = m_views.find(pView->getID());
	if (view != m_views.end())
	{
		m_views.erase(view);
	}
}

void Model::DeviceModel::clearViews()
{
	m_views.clear();
}

void Model::DeviceModel::updateViews(const Hint::HintBase& hint)
{
	for (auto& viewIter : m_views)
	{
		if (viewIter.second->canUpdate())
		{
			viewIter.second->update(hint);
		}
	}
}

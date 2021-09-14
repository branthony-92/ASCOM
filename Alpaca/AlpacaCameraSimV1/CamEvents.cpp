#include "stdafx.h"
#include "CamEvents.h"

TErrorEventPtr Camera::Event::createErrorEvent()
{
	return std::make_shared<FailEvent>();
}

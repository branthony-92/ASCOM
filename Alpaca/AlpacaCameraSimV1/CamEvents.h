#ifndef CAMEVENTS_H
#define CAMEVENTS_H

#include "SMEvent.h"

namespace Camera {
namespace Event {

enum class EventID
{
	CamEvent_Unknown = -1,
	CamEvent_Success,
	CamEvent_Failure,
	CamEvent_Connect,
	CamEvent_StartExposure,
	CamEvent_StopExposure,
	CamEvent_AbortExposure,
	CamEvent_TransferImage,
	CamEvent_PulseGuide,
	CamEvent_Disconnect,
	NumEvents,
};

class FailEvent : public CSMErrorEvent
{
public:
	FailEvent()
		: CSMErrorEvent(static_cast<uint32_t>(EventID::CamEvent_Failure), "CamEvent_Failure")
	{}
};

template <EventID ID>
class CameraEvent : public CSMEvent
{
public:
	CameraEvent(std::string name)
		: CSMEvent(static_cast<uint32_t>(ID), name)
	{}
	virtual ~CameraEvent() {}

};

typedef CameraEvent<EventID::CamEvent_Connect>		  CamEventConnect;
typedef CameraEvent<EventID::CamEvent_Success>        CamEventSuccess;
typedef CameraEvent<EventID::CamEvent_StartExposure>  CamEventStartExposure;
typedef CameraEvent<EventID::CamEvent_StopExposure>   CamEventStopExposure;
typedef CameraEvent<EventID::CamEvent_AbortExposure>  CamEventAbortExposure;
typedef CameraEvent<EventID::CamEvent_TransferImage>  CamEvenTransferImage;
typedef CameraEvent<EventID::CamEvent_PulseGuide>     CamEventPulseGuide;
typedef CameraEvent<EventID::CamEvent_Disconnect>     CamEventDisconnect;

typedef std::shared_ptr<CamEventConnect>         CamEventConnectPtr;
typedef std::shared_ptr<CamEventSuccess>         CamEventSuccessPtr;
typedef std::shared_ptr<CamEventStartExposure>	 CamEventStartExposurePtr;
typedef std::shared_ptr<CamEventStopExposure>	 CamEventStopExposurePtr;
typedef std::shared_ptr<CamEventAbortExposure>	 CamEventAbortExposurePtr;
typedef std::shared_ptr<CamEventPulseGuide>		 CamEventPulseGuidePtr;
typedef std::shared_ptr<CamEvenTransferImage>	 CamEventTransferImagePtr;
typedef std::shared_ptr<CamEventDisconnect>		 CamEventDisconnectPtr;
		
// fail event
typedef std::shared_ptr<FailEvent>				 CamEventFailurePtr;

// Event fatory method
template <EventID ID>
std::shared_ptr<CameraEvent<ID>> create()
{
	const std::map<EventID, std::string> c_eventReflectionMap = {
		{ EventID::CamEvent_Unknown,         "CamEvent_Unknown"        },
		{ EventID::CamEvent_Success,		 "CamEvent_Success"        },
		{ EventID::CamEvent_Failure,		 "CamEvent_Failure"        },
		{ EventID::CamEvent_Connect,		 "CamEvent_Connect"        },
		{ EventID::CamEvent_StartExposure,	 "CamEvent_StartExposure"  },
		{ EventID::CamEvent_StopExposure,	 "CamEvent_StopExposure"   },
		{ EventID::CamEvent_AbortExposure,   "CamEvent_AbourtExposure"  },
		{ EventID::CamEvent_PulseGuide,	     "CamEvent_PulseGuide"     },
		{ EventID::CamEvent_TransferImage,	 "CamEvent_TransferImage"  },
		{ EventID::CamEvent_Disconnect,	     "CamEvent_Disconnect"	   }
	};

	auto name = c_eventReflectionMap.at(ID);

	auto pEvent = std::make_shared<CameraEvent<ID>>(name);

	return pEvent;
}

TErrorEventPtr createErrorEvent();
}
}

#endif // !CAMEVENTS_H
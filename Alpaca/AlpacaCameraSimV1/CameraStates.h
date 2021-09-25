#ifndef CAMERASTATES_H
#define CAMERASTATES_H

#include "SMBase.h"
#include "CamEvents.h"

namespace Camera {
namespace States {

	// each camera state will inherit from the SMBase class so we can treat them as a state or state machine
	class CameraState : public CSMBase
	{
	public:
		enum class StateID
		{
			CamState_Unknown = -1,
	
			CamState_FirstState,

			CamState_NotConnected = CamState_FirstState,
			CamState_Connecting,
			CamState_Idle,
			
			CamState_Exposing,
			// Exposing substates


			CamState_Transferring,
			CamState_PulseGuiding,
			CamState_Disconnecting,

			CamState_Error,

			NumBaseStates,

			CamState_Exposing_FirstSubstate,
			CamState_Exposing_GeneratingBackground = CamState_Exposing_FirstSubstate,
			CamState_Exposing_GeneratingImage,
			CamState_Exposing_Stopped,
			CamState_Exposing_LastSubstate = CamState_Exposing_Stopped,
		
		};

		CameraState(StateID ID, std::string name)
			: CSMBase(static_cast<uint32_t>(ID), name)
		{}
		virtual ~CameraState() {}

		void addTransition(Event::EventID eventID, StateID targetStateID);
	};
	typedef std::shared_ptr<CameraState> CamStatePtr;

	class CameraState_Unknown : public CameraState
	{
	public:
		CameraState_Unknown()
			: CameraState(CameraState::StateID::CamState_Unknown, "CamState_Unknown")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Unknown> UnknownStatePtr;

	class CameraState_NotConnected : public CameraState
	{
	public:
		CameraState_NotConnected()
			: CameraState(CameraState::StateID::CamState_NotConnected, "CamState_NotConnected")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_NotConnected> NotConnectedStatePtr;

	class CameraState_Connecting : public CameraState
	{
	public:
		CameraState_Connecting()
			: CameraState(CameraState::StateID::CamState_Connecting, "CamState_Connecting")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Connecting> ConnectingStatePtr;

	class CameraState_Idle : public CameraState
	{
	public:
		CameraState_Idle()
			: CameraState(CameraState::StateID::CamState_Idle, "CamState_Idle")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Connecting> ConnectingStatePtr;

	class CameraState_Exposing : public CameraState
	{
	public:
		CameraState_Exposing()
			: CameraState(CameraState::StateID::CamState_Exposing, "CamState_Exposing")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Exposing> ExposingStatePtr;

	class CameraState_Transferring : public CameraState
	{
	public:
		CameraState_Transferring()
			: CameraState(CameraState::StateID::CamState_Transferring, "CamState_Exposing_Transferring")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Transferring> TransferringStatePtr;


	class CameraState_PulseGuiding : public CameraState
	{
	public:
		CameraState_PulseGuiding()
			: CameraState(CameraState::StateID::CamState_PulseGuiding, "CamState_PulseGuiding")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_PulseGuiding> PulseGuidingStatePtr;

	class CameraState_Disconnecting : public CameraState
	{
	public:
		CameraState_Disconnecting()
			: CameraState(CameraState::StateID::CamState_Disconnecting, "CamState_Disconnecting")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Disconnecting> DisconnectingStatePtr;

	class CameraState_Error : public CameraState
	{
	public:
		CameraState_Error()
			: CameraState(CameraState::StateID::CamState_Error, "CamState_Error")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Error> ErrorStatePtr;

	class CameraState_Exposing_GeneratingBackground : public CameraState
	{
	public:
		CameraState_Exposing_GeneratingBackground()
			: CameraState(CameraState::StateID::CamState_Exposing_GeneratingBackground, "CamState_Exposing_GeneratingBackground")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Exposing_GeneratingBackground> ExposingState_GeneratingBackgroundPtr;

	class CameraState_Exposing_GeneratingImage : public CameraState
	{
	public:
		CameraState_Exposing_GeneratingImage()
			: CameraState(CameraState::StateID::CamState_Exposing_GeneratingImage, "CamState_Exposing_GeneratingImage")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Exposing_GeneratingImage> ExposingState_GeneratingImgPtr;

	class CameraState_Exposing_Stopped : public CameraState
	{
	public:
		CameraState_Exposing_Stopped()
			: CameraState(CameraState::StateID::CamState_Exposing_Stopped, "CamState_Exposing_Stopped")
		{}

		virtual bool enterState(TSMContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TSMContextPtr pCtx) override;
		virtual bool exitState(TSMContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Exposing_Stopped> ExposingState_StoppedPtr;

	CamStatePtr createState(CameraState::StateID);
}
}
#endif // !CAMERASTATES_H
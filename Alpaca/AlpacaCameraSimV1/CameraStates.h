#ifndef CAMERASTATES_H
#define CAMERASTATES_H

#include "SMBase.h"

namespace Camera {

	// each camera state will inherit from the SMBase class so we can treat them as a state or state machine
	class CameraState : public CSMBase
	{
	public:
		enum class StateID
		{
			CamState_Unknown = -1,
	
			CamState_NotConnected,
			CamState_Connecting,
			CamState_Idle,
			CamState_Exposing,
			CamState_Error,
		
			eNumStates
		};

		CameraState(StateID ID, std::string name)
			: CSMBase(static_cast<uint32_t>(ID), name)
		{}
		virtual ~CameraState() {}
	};
	typedef std::shared_ptr<CameraState> CamStatePtr;

	class CameraState_Unknown : public CameraState
	{
	public:
		CameraState_Unknown()
			: CameraState(CameraState::StateID::CamState_Unknown, "CamState_Unknown")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Unknown> UnknownStatePtr;

	class CameraState_NotConnected : public CameraState
	{
	public:
		CameraState_NotConnected()
			: CameraState(CameraState::StateID::CamState_NotConnected, "CamState_NotConnected")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_NotConnected> NotConnectedStatePtr;

	class CameraState_Connecting : public CameraState
	{
	public:
		CameraState_Connecting()
			: CameraState(CameraState::StateID::CamState_Connecting, "CamState_Connecting")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Connecting> ConnectingStatePtr;

	class CameraState_Idle : public CameraState
	{
	public:
		CameraState_Idle()
			: CameraState(CameraState::StateID::CamState_Idle, "CamState_Idle")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Connecting> ConnectingStatePtr;

	class CameraState_Exposing : public CameraState
	{
	public:
		CameraState_Exposing()
			: CameraState(CameraState::StateID::CamState_Exposing, "CamState_Exposing")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Exposing> ExposingStatePtr;

	class CameraState_Error : public CameraState
	{
	public:
		CameraState_Error()
			: CameraState(CameraState::StateID::CamState_Error, "CamState_Error")
		{}

		virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
		virtual bool ticState(TContextPtr pCtx) override;
		virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

		virtual void initTransitionTable() override;
	};
	typedef std::shared_ptr<CameraState_Error> ErrorStatePtr;
}
#endif // !CAMERASTATES_H
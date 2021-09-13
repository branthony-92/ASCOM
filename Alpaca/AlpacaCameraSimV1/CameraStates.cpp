#include "stdafx.h"
#include "CameraStates.h"

bool Camera::CameraState_Unknown::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_Unknown::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_Unknown::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_Unknown::initTransitionTable()
{
}

bool Camera::CameraState_NotConnected::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_NotConnected::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_NotConnected::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_NotConnected::initTransitionTable()
{
}

bool Camera::CameraState_Connecting::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_Connecting::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_Connecting::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_Connecting::initTransitionTable()
{
}

bool Camera::CameraState_Idle::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_Idle::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_Idle::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_Idle::initTransitionTable()
{
}

bool Camera::CameraState_Exposing::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_Exposing::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_Exposing::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_Exposing::initTransitionTable()
{
}

bool Camera::CameraState_Error::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

bool Camera::CameraState_Error::ticState(TContextPtr pCtx)
{
	return false;
}

bool Camera::CameraState_Error::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return false;
}

void Camera::CameraState_Error::initTransitionTable()
{
}

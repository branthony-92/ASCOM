#ifndef CAMERA_H
#define CAMERA_H

#include "RESTServerContext.h"
#include "IAlpacaCamV1.h"
#include "SMContext.h"

class CameraV1 
	: public Alpaca::Camera::IAlpacaCamV1
	, public CSMStateContext
{
public:
	CameraV1(unsigned int deviceIndex);
	virtual ~CameraV1();

	// Common Device methods
	virtual std::string action(std::string action, std::string parameters) override;

	virtual void commandBlind(std::string cmd, std::string raw) override;
	virtual bool commandBool(std::string cmd, std::string raw) override;
	virtual std::string commandString(std::string cmd, std::string raw) override;

	// Camera methods
	virtual void startExposure(double duration, bool isLight) override;
	virtual void stopExposure() override;
	virtual void abortExposure() override;
	virtual void pulseGuide(Alpaca::Camera::PulseGuideDirection direction, int duration) override;

private:

	void generateImage();
public:
	
};


#endif // !CAMERA_H
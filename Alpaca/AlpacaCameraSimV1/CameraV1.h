#ifndef CAMERA_H
#define CAMERA_H

#include "RESTServerContext.h"
#include "IAlpacaCamV1.h"
#include "SMContext.h"
#include "DeviceModel.h"

namespace Camera {

class CameraV1 
	: public Alpaca::Camera::IAlpacaCamV1
	, public Model::DeviceModel
	, public CSMStateContext
{
	DEVICEPROP(bool, GenerateBackground, generateBackground)
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

	virtual void connect() override;
	virtual void disconnect() override;
	virtual void setupCamera() override;
	virtual void shutdownCamera() override;
	virtual void finishExposure() override;

	virtual void generateImgBackground();
	virtual void generateImage();
	virtual void transferImage();

	void recoverState();

	void onStart();
	void onIdle();
private:


public:
	
};
typedef std::shared_ptr<CameraV1> CamPtr;

class ExposureManager : public CSMStateContext
{
	DEVICEPROP(bool, IsComplete, isComplete)
public:
	ExposureManager(CamPtr pCam)
		: CSMStateContext("Exposure Manager")
		, m_isComplete(false)
	{}

	//void on
};

}
#endif // !CAMERA_H
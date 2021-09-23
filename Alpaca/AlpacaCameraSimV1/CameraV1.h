#ifndef CAMERA_H
#define CAMERA_H

#include "RESTServerContext.h"
#include "IAlpacaCamV1.h"
#include "SMContext.h"
#include "DeviceModel.h"

#include <thread>

namespace Camera {

class CameraV1 
	: public Alpaca::Camera::IAlpacaCamV1
	, public Model::DeviceModel
	, public CSMStateContext
{
	DEVICEPROP(bool, GenerateBackground, generateBackground)
	DEVICEPROP(uint32_t, BufferReadOffset, bufferReadOffset)
	DEVICEPROP(Alpaca::Image::ImageType, ImageType, imageType);

public:
	CameraV1(unsigned int deviceIndex, std::string name);
	virtual ~CameraV1();

	// Server context Overrides
	void onServerStart() override;
	void onServerReset()    override;
	void onServerShutdown() override;
	std::string findResource(std::string target) override;


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
	void initImg2D();
	void initImg3D();
	virtual void transferImage() override;
	bool readChunk(uint32_t chunkSizeBytes = 2048);
	virtual void finishExposure() override;

	virtual void generateImgBackground();
	void generateStar(const unsigned int planeNum, const unsigned int starX, const unsigned int starY);
	virtual void generateImage();

	void recoverState();

	void onStart();
	void onIdle();

	
private:

	void reframeImg();
	void applyBinning();

public:
	

};
typedef std::shared_ptr<CameraV1> CamPtr;


class ExposureManager : public CSMStateContext
{
	DEVICEPROP(bool, IsComplete, isComplete)
	DEVICEPROP(CamPtr, Cam, pCam)

	std::thread m_worker;
public:
	ExposureManager(CamPtr pCam)
		: CSMStateContext("Exposure Manager")
		, m_isComplete(false)
		, m_pCam(pCam)
		, m_worker()
	{}
	
	virtual ~ExposureManager();

	//void on
	void onGenerateBackground();
	void onGenerateImg();
};

}
#endif // !CAMERA_H
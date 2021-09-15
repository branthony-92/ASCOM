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


public:
	
	template<typename T>
	T binPixels(const std::shared_ptr<Alpaca::Image::Buffer> pBuf, 
		unsigned int x,     unsigned int y, 
		unsigned int binX,  unsigned int binY, 
		unsigned int width, unsigned int height);

	template<typename T>
	void applyBinning();

};
typedef std::shared_ptr<CameraV1> CamPtr;



template <typename T>
T Camera::CameraV1::binPixels(const std::shared_ptr<Alpaca::Image::Buffer> pBuf,
	unsigned int x,     unsigned int y,
	unsigned int binX,  unsigned int binY,
	unsigned int width, unsigned int height)
{
	T sum = 0;

	// ensure our mask stays in range
	const unsigned int startX = std::max(0u, x);
	const unsigned int startY = std::max(0u, y);
	const unsigned int endX = std::min((x + binX), width);
	const unsigned int endY = std::min((y + binY), height);

	for (auto _y = startY; _y < endY; _y++)
	{
		for (auto _x = startX; _x < endX; _x++)
		{
			switch (pBuf->getType())
			{
			case Alpaca::Image::ImageType::Int16:
				sum += pBuf->getData()[_x + _y * width].i16;
				break;
			case Alpaca::Image::ImageType::Int32:
				sum += pBuf->getData()[_x + _y * width].i32;
				break;
			case Alpaca::Image::ImageType::Float64:
				sum += pBuf->getData()[_x + _y * width].f64;
				break;
			default:
				break;
			}
		}
	}
	unsigned int numPixels = binX * binY;
	unsigned int avg = sum / numPixels;

	// clamp to the max pixel value to prevent overflows
	avg = std::min(avg, static_cast<unsigned int>(USHRT_MAX));

	return static_cast<T>(avg);
};

template <typename T>
void Camera::CameraV1::applyBinning()
{
	for (auto& plane : m_pImageArray->getPlanes())
	{
		const unsigned int xTarget = m_numX / m_binX;
		const unsigned int yTarget = m_numY / m_binY;

		const unsigned int targetLen = xTarget * yTarget;

		const unsigned int xEnd = m_numX;
		const unsigned int yEnd = m_numY;

		const unsigned int xSkip = m_binX;
		const unsigned int ySkip = m_binY;

		unsigned int outX = 0;
		unsigned int outY = 0;

		auto pDestBuffer = new T[xTarget * yTarget];

		auto pSourceBuffer = plane.getImageBuffer();
		for (auto y = 0u; y < yEnd; y += ySkip)
		{
			outX = 0;
			for (auto x = 0u; x < xEnd; x += xSkip)
			{
				pDestBuffer[outX + outY * xTarget] = binPixels<T>(pSourceBuffer, x, y, m_binX, m_binY, m_numX, m_numY);
				if (++outX >= xTarget) break;
			}
			if (++outY >= yTarget) break;
		}
		plane.getImageBuffer()->moveToBuffer(pDestBuffer, targetLen, pSourceBuffer->getType());
	}
}

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
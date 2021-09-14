#ifndef IALPACAMV1_H
#define IALPACAMV1_H

#include "AlpacaDeviceV1.h"
#include "AlpacaImage.h"

namespace Alpaca{
namespace Camera {

	enum class SensorTypeID
	{
		Mono,   // Monochrome, 
		Colour,	// Colour not requiring Bayer decoding 
		RGGB,	// RGGB Bayer encoding 
		CMYG,	// CMYG Bayer encoding 
		CMYG2,	// CMYG2 Bayer encoding 
		LRGB,	// LRGB TRUESENSE Bayer encoding. 
	};

	enum class CameraStateID
	{
		CameraIdle, 
		CameraWaiting, 
		CameraExposing, 
		CameraReading, 
		CameraDownload, 
		CameraError
	};

	enum class PulseGuideDirection
	{
		Unknown = -1,
		North,
		South,
		East,
		West,
	};

class IAlpacaCamV1 : public AlpacaDeviceV1
{
	DEVICEPROP(int,		        BayerOffsetX,	       bayerOffsetX)
	DEVICEPROP(int,		        BayerOffsetY,          bayerOffsetY)
	DEVICEPROP(int,		        BinX,                  binX)
	DEVICEPROP(int,		        BinY,                  binY)
	DEVICEPROP(CameraStateID,	CameraState,           cameraState)
	DEVICEPROP(int,		        CameraXSize,           cameraXSize)
	DEVICEPROP(int,		        CameraYSize,           cameraYSize)
	DEVICEPROP(bool,            CanAbortExposure,      canAbortExposure)
	DEVICEPROP(bool,	        CanAsymmetricBin,      canAsymmetricBin)
	DEVICEPROP(bool,	        CanFastReadout,        canFastReadout)
	DEVICEPROP(bool,	        CanGetCoolerPower,     canGetCoolerPower)
	DEVICEPROP(bool,	        CanPulseGuide,	       canPulseGuide)
	DEVICEPROP(bool,	        CanSetCCDTemp,	       canSetCCDTemp)
	DEVICEPROP(bool,	        CanStopExposure,       canStopExposure)
	DEVICEPROP(double,          CCDTemp,			   CCDTemp)
	DEVICEPROP(bool,            CoolerOn,		       coolerOn)
	DEVICEPROP(double,	        CoolerPower,		   coolerPower)
	DEVICEPROP(double,          ElectrosPerAdu,        electrosPerAdu)
	DEVICEPROP(double,	        ExposureMin,		   exposureMin)
	DEVICEPROP(double,	        ExposureMax,		   exposureMax)
	DEVICEPROP(double,	        ExposureResolution,    eposureResolution)
	DEVICEPROP(bool,	        FastReadout,		   fastReadout)
	DEVICEPROP(double,	        FullWellCapacity,      fullWellCapacity)
	DEVICEPROP(int,		        Gain,			       gain)
	DEVICEPROP(int,		        GainMax,		       gainMax)
	DEVICEPROP(int,		        GainMin,			   gainMin)
	DEVICEPROP(StringList,      Gains,                 gains)
	DEVICEPROP(bool,	        HasShutter,		       hasShutter)
	DEVICEPROP(double,          HeatsinkTemp,	       heatsinkTemp)
	DEVICEPROP(bool,	        ImageReady,		       imageReady)
	DEVICEPROP(bool,	        IsPulseGuiding,	       isPulseGuiding)
	DEVICEPROP(double,	        LastExposureDuration,  lastExposureDuration)
	DEVICEPROP(std::string,     LastExposureStartTime, lastExposureStartTime)
	DEVICEPROP(int,			    MaxADU,				   maxADU)
	DEVICEPROP(int,			    MaxBinX,			   maxBinX)
	DEVICEPROP(int,			    MaxBinY,			   maxBinY)
	DEVICEPROP(int,			    NumX,				   numX)
	DEVICEPROP(int,			    NumY,				   numY)
	DEVICEPROP(int,			    Offset,				   offset)
	DEVICEPROP(int,			    OffsetMin,			   offsetMin)
	DEVICEPROP(int,			    OffsetMax,			   offsetMax)
	DEVICEPROP(StringList,      Offsets,			   offsets)
	DEVICEPROP(int,			    PercentComplete,	   percentComplete)
	DEVICEPROP(double,		    PixelSizeX,			   pixelSizeX)
	DEVICEPROP(double,		    PixelSizeY,			   pixelSizeY)
	DEVICEPROP(int,			    ReadoutMode,		   readoutMode)
	DEVICEPROP(StringList,      ReadoutModes,		   readoutModes)
	DEVICEPROP(std::string,     SensorName,			   sensorName)
	DEVICEPROP(SensorTypeID,	SensorType,		       sensorType)
	DEVICEPROP(double,			CoolerSetpoint,		   coolerSetpoint)
	DEVICEPROP(int,			    StartX,				   startX)
	DEVICEPROP(int,				StartY,				   startY)
	DEVICEPROP(double,			SubExposureDuration,   subExposureDuration)

	// One of each possible image types
	DEVICEPROP(Image::ImageDataPtr, ImageArray, pImageArray)

public:
	IAlpacaCamV1(unsigned int deviceNum, std::string deviceName);
	virtual ~IAlpacaCamV1();
	void initCamHandlers();

// virtual camera methods
public:
	virtual void startExposure(double duration, bool isLight) = 0;
	virtual void stopExposure()  = 0;
	virtual void abortExposure() = 0;
	virtual void pulseGuide(PulseGuideDirection direction, int duration) = 0;
	virtual void setupCamera() = 0;
	virtual void shutdownCamera() = 0;
	virtual void finishExposure() = 0;
	virtual void transferImage() = 0;

};

}
}
#endif // !IALPACAMV1_H
#ifndef ALPACAIMAGE_H
#define ALPACAIMAGE_H

#include "AlpacaUtils.h"

#include <vector>
#include <memory>

namespace Alpaca {
namespace Image  {
	enum class ImageType
	{
		UnknownType,
		Int16,
		Int32,
		Float64
	};

	class Buffer
	{
		union VarData
		{
			short  i16;
			int    i32;
			double f64;
		};

		DEVICEPROP(VarData*,  Data,    pData)
		DEVICEPROP(uint32_t,  Length,  length)
		DEVICEPROP(ImageType, Type,    type)
	public:
		Buffer()
			: m_pData(nullptr)
			, m_length(0)
		{}

		Buffer(uint32_t len, ImageType type)
			: m_pData(new VarData[len])
			, m_length(len)
			, m_type(type)
		{}

		Buffer(void* pBuf, uint32_t len, ImageType type)
			: m_pData(nullptr)
			, m_length(0)
		{
			copyToBuffer(pBuf, len, type);
		}

		~Buffer() { reset(); }

		// accesors
		void reset()
		{
			if (m_pData)
			{
				//LOGDEALLOC
				delete[] m_pData;
			}
			m_length = 0;
		}

		void reset(unsigned int newLen)
		{
			reset();
			m_pData = new VarData[newLen];
			m_length = newLen;
		}

		void copyToBuffer(void* pBuf, uint32_t len, ImageType type)
		{
			reset(len);
			switch (type)
			{
			case Alpaca::Image::ImageType::UnknownType:
			case Alpaca::Image::ImageType::Int16:
				for (auto i = 0; i < len; i++)
				{
					m_pData[i].i16 = static_cast<short*>(pBuf)[i];
				}
				break;
			case Alpaca::Image::ImageType::Int32:
				for (auto i = 0; i < len; i++)
				{
					m_pData[i].i32 = static_cast<int*>(pBuf)[i];
				}
				break;
			case Alpaca::Image::ImageType::Float64:
				for (auto i = 0; i < len; i++)
				{
					m_pData[i].f64 = static_cast<double*>(pBuf)[i];
				}
				break;
			default:
				break;
			}
			m_type   = type;
			m_length = len;
		}

		void moveToBuffer(void* pBuf, uint32_t len, ImageType type)
		{
			copyToBuffer(pBuf, len, type);
			delete pBuf;
			pBuf = nullptr;
		}
	};

	class ImagePlane
	{
	public:
		ImagePlane()
			: m_pImageBuffer(nullptr)
		{}

		ImagePlane(void* pBuf, size_t len, ImageType type)
			: m_pImageBuffer(std::make_shared<Buffer>(pBuf, len, type))
		{}
	
		bool isValid() const { 
			if (!m_pImageBuffer) return false;
			if (m_pImageBuffer->getLength() == 0) return false;
		}
		DEVICEPROP(std::shared_ptr<Buffer>, ImageBuffer, pImageBuffer)

	public:
		void setBuffer(void* pBuf, size_t len, ImageType type)
		{
			m_pImageBuffer = std::make_shared<Buffer>(pBuf, len, type);
		}
	};

	class ImageData
	{
	public:

		ImageData()
			: m_type()
			, m_rank(0)
			, m_planes()
			, m_xSize(0)
			, m_ySize(0)
		{}
	 
		bool validate()
		{
			const auto len = m_xSize * m_ySize;
			if (m_planes.empty()) return false;
			for (ImagePlane& plane : m_planes)
			{
				if (!plane.isValid()) return false;                            // plane buffer must exist and nuffer length must be > 0
				if (plane.getImageBuffer()->getLength() != len)  return false; // plane lengths must match
				if (plane.getImageBuffer()->getType() != m_type) return false; // plane types must match
			}
			return true;
		}

		DEVICEPROP(unsigned int,            Rank,   rank)
		DEVICEPROP(ImageType,               Type,   type)
		DEVICEPROP(unsigned int,            XSize,  xSize)
		DEVICEPROP(unsigned int,            YSize,  ySize)
		DEVICEPROP(std::vector<ImagePlane>,	Planes, planes)

	public:
		void addPlane(ImagePlane plane) { m_planes.push_back(plane); }
	};
	typedef std::shared_ptr<ImageData> ImageDataPtr;

} // End Image
} // End Alpaca
#endif // !ALPACAIMAGE_H
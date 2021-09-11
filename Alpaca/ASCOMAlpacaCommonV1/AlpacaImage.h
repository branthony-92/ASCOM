#ifndef ALPACAIMAGE_H
#define ALPACAIMAGE_H

#include <vector>
#include <memory>

namespace Alpaca {
namespace Image  {
	enum class ImageType
	{
		UknownType,
		Int16,
		Int32,
		Float64
	};


	template <class DataType, ImageType imgType>
	class ImageData
	{
	public:
		class Buffer
		{
			DataType* m_pData;
			uint32_t  m_length;

		public:
			Buffer()
				: m_pData(nullptr)
				, m_length(0)
			{}

			Buffer(uint32_t len)
				: m_pData(new DataType[len])
				, m_length(len)
			{}

			Buffer(DataType* pBuf, uint32_t len)
				: m_pData(pBuf)
				, m_length(len)
			{}

			~Buffer() { reset(); }

			// accesors

			DataType* data() { return m_pData; }
			uint32_t length() const { return m_length; }

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
				m_pData = new DataType[newLen];
				m_length = newLen;
			}

			void copyToBuffer(DataType* pBuf, uint32_t len)
			{
				reset();
				m_pData = pBuf;
				m_length = len;
			}

			void moveToBuffer(DataType* pBuf, uint32_t len)
			{
				reset();
				m_pData = new DataType[len];
				memcpy(m_pData, pBuf, len * sizeof(DataType));
				m_length = len;
				delete pBuf;
				pBuf = nullptr;
			}
		};
		typedef std::shared_ptr<Buffer> BufferPtr;

		class ImagePlane
		{
		public:
			ImagePlane()
				: m_pImageBuffer(nullptr)
				, m_xSize(0)
				, m_ySize(0)
			{}
			ImagePlane(unsigned short xSize, unsigned short ySize)
				: m_pImageBuffer(xSize * ySize)
				, m_xSize(xSize)
				, m_ySize(ySize)
			{}

			BufferPtr    m_pImageBuffer;
			unsigned int m_xSize;
			unsigned int m_ySize;

			bool isValid() const { return m_xSize > 0 || m_ySize > 0; }
		};

		ImageData()
			: m_type(imgType)
			, m_rank(0)
			, m_layers()
		{}
	 
	private:
		unsigned int                   m_rank;
		ImageType                      m_type;
		std::vector<ImagePlane>		   m_layers;
	};

	typedef ImageData<int,    ImageType::UknownType>  UnknownImage;
	typedef ImageData<short,  ImageType::Int16>       I16Image;
	typedef ImageData<int,    ImageType::Int32>       I32Image;
	typedef ImageData<double, ImageType::Float64>     F64Image;

	typedef std::shared_ptr<I16Image> I16ImagePtr;
	typedef std::shared_ptr<I32Image> I32ImagePtr;
	typedef std::shared_ptr<F64Image> F64ImagePtr;
} // End Image
} // End Alpaca
#endif // !ALPACAIMAGE_H
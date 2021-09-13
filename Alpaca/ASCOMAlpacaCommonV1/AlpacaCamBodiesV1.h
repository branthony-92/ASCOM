#pragma once

#include "AlpacaResponseBodiesV1.h"
#include "AlpacaImage.h"
#include "ErrorCodes.h"
#include <memory>

namespace Alpaca {
namespace Bodies {

class ImageResponse : public JSONInfoBody
{
	using Int32List1D = std::vector<int>;
	using Int32List2D = std::vector<Int32List1D>;
	using Int32List3D = std::vector<Int32List2D>;

	using DoubleList1D = std::vector<double>;
	using DoubleList2D = std::vector<DoubleList1D>;
	using DoubleList3D = std::vector<DoubleList2D>;

	DEVICEPROP(Image::ImageType,     Type,			      type)
	DEVICEPROP(int,                  Rank,			      rank)
	DEVICEPROP(Image::ImageDataPtr,	 ImageData,           pImageData)
	DEVICEPROP(unsigned int,         ClientTransactionID, clientTransactionID)
	DEVICEPROP(unsigned int,         ServerTransactionID, serverTransactionID)
	DEVICEPROP(int,			         ErrorNumber,         errorNumber)
	DEVICEPROP(std::string,          ErrorMessage,        errorMessage)

public:
	ImageResponse()
		: JSONInfoBody("ValueResponse")
		, m_type(Image::ImageType::UnknownType)
		, m_rank(0)
		, m_pImageData(nullptr)
		, m_clientTransactionID(0)
		, m_serverTransactionID(0)
		, m_errorNumber(0)
		, m_errorMessage()
	{}
	virtual ~ImageResponse() {}

private:
	template <typename PixelType>
	void writeImgArray(JSON& info) const
	{
		using Array1D = std::vector<PixelType>;
		using Array2D = std::vector<Array1D>;
		using Array3D = std::vector<Array2D>;

		// short image data
		if (!(m_pImageData && m_pImageData->validate()))
		{
			return;
		}
		auto xSize  = m_pImageData->getXSize();
		auto ySize  = m_pImageData->getYSize();
		auto planes = m_pImageData->getPlanes();

		auto imgTypeID = static_cast<Image::ImageType>(m_type);

		switch (m_rank)
		{
		case 2:
		{
			Array2D output;

			// 2D array
			auto plane = planes.front();
			auto pPlaneBuf = plane.getImageBuffer()->getData();
			output.resize(ySize);

			for (auto y = 0u; y < ySize; y++)
			{
				output[y].resize(xSize);
				for (auto x = 0u; x < xSize; x++)
				{
					if (imgTypeID == Image::ImageType::Float64)
					{
						output[y][x] = pPlaneBuf[x + y * xSize].f64;
					}
					else if (imgTypeID == Image::ImageType::Int32)
					{
						output[y][x] = pPlaneBuf[x + y * xSize].i32;
					}
					else
					{
						output[y][x] = pPlaneBuf[x + y * xSize].i16;
					}
				}
			}

			info["Value"] = output;
			break;
		}
		case 3:
		{
			Array3D output;

			auto plane1 = planes[0];
			auto plane2 = planes[1];
			auto plane3 = planes[2];

			auto pPlaneBuf1 = plane1.getImageBuffer()->getData();
			auto pPlaneBuf2 = plane2.getImageBuffer()->getData();
			auto pPlaneBuf3 = plane3.getImageBuffer()->getData();

			output.resize(ySize);

			for (auto y = 0u; y < ySize; y++)
			{
				output[y].resize(xSize);
				for (auto x = 0u; x < xSize; x++)
				{
					output[y][x].resize(3);
					if (imgTypeID == Image::ImageType::Float64)
					{
						output[y][x][0] = pPlaneBuf1[x + y * xSize].f64;
						output[y][x][1] = pPlaneBuf2[x + y * xSize].f64;
						output[y][x][2] = pPlaneBuf3[x + y * xSize].f64;
					}
					else if (imgTypeID == Image::ImageType::Int32)
					{
						output[y][x][0] = pPlaneBuf1[x + y * xSize].i32;
						output[y][x][1] = pPlaneBuf2[x + y * xSize].i32;
						output[y][x][2] = pPlaneBuf3[x + y * xSize].i32;
					}
					else
					{
						output[y][x][0] = pPlaneBuf1[x + y * xSize].i16;
						output[y][x][1] = pPlaneBuf2[x + y * xSize].i16;
						output[y][x][2] = pPlaneBuf3[x + y * xSize].i16;;
					}
				}
			}

			info["Value"] = output;
			break;
		}
		default:
			break;
		}
	}

	template <typename PixelType>
	void readImgArray(JSON& info) 
	{
		using Array1D = std::vector<PixelType>;
		using Array2D = std::vector<Array1D>;
		using Array3D = std::vector<Array2D>;

		auto imgTypeID = static_cast<Image::ImageType>(m_type);

		switch (m_rank)
		{
		case 2:
		{
			Array2D output = jsonUtils::extractValue<Array2D>(info, "Value", Array2D());

			if (output.empty())
			{
				m_pImageData = nullptr;
				return;
			}

			const auto ySize = output.size();
			const auto xSize = output.front().size();
			const auto len   = xSize * ySize;
			PixelType* pBuf = new PixelType[len];
			for (Array1D& xAxis : output)
			{
				if (xAxis.size() != xSize)
				{
					m_pImageData = nullptr;
					return;
				}
				for (PixelType& val : xAxis)
				{
					// read the plane
					*pBuf++ = val;
				}
			}
			Image::ImagePlane plane(pBuf, len, imgTypeID);
			m_pImageData->addPlane(plane);
			m_pImageData->setXSize(xSize);
			m_pImageData->setYSize(ySize);
			delete[] pBuf;
			break;
		}
		case 3:
		{
			Array3D output = jsonUtils::extractValue<Array3D>(info, "Value", Array3D());

			const auto ySize = output.size();
			const auto xSize = output.front().size();

			const auto len = xSize * ySize;

			PixelType* pPlaneBuf1 = new PixelType[len];
			PixelType* pPlaneBuf2 = new PixelType[len];
			PixelType* pPlaneBuf3 = new PixelType[len];

			for (Array2D& xAxis : output)
			{
				if (xAxis.size() != xSize)
				{
					m_pImageData = nullptr;
					return;
				}
				for (Array1D& vals : xAxis)
				{
					if (vals.size() != 3)
					{
						m_pImageData = nullptr;
						return;
					}
					// read the planes
					*pPlaneBuf1++ = vals[0];
					*pPlaneBuf2++ = vals[1];
					*pPlaneBuf3++ = vals[2];
				}
			}
			Image::ImagePlane plane1(pPlaneBuf1, len, imgTypeID);
			Image::ImagePlane plane2(pPlaneBuf2, len, imgTypeID);
			Image::ImagePlane plane3(pPlaneBuf3, len, imgTypeID);

			m_pImageData->addPlane(plane1);
			m_pImageData->addPlane(plane2);
			m_pImageData->addPlane(plane3);

			m_pImageData->setXSize(xSize);
			m_pImageData->setYSize(ySize);

			delete[] pPlaneBuf1;
			delete[] pPlaneBuf2;
			delete[] pPlaneBuf3;
			break;
		}
		default:
			break;
		}
	}
public:

	virtual JSON toJSON() const override
	{
		JSON info = JSON::object();

		switch (m_type)
		{
			case Image::ImageType::Int16:       
				writeImgArray<short>(info);  
				break;
			case Image::ImageType::UnknownType:
			case Image::ImageType::Int32:       
				writeImgArray<int>(info);    
				break;
			case Image::ImageType::Float64:     
				writeImgArray<double>(info); 
				break;
			default: break;
		}
		
		info["Type"]                = m_type;
		info["Rank"]                = m_rank;
		info["ClientTransactionID"] = m_clientTransactionID;
		info["ServerTransactionID"] = m_serverTransactionID;
		info["ErrorNumber"]         = m_errorNumber;
		info["ErrorMessage"]        = m_errorMessage;

		return info;
	}
	virtual void loadJSON(JSON info) override
	{
		m_type = jsonUtils::extractValue<Image::ImageType, int>(info, "Type", Image::ImageType::UnknownType);
		m_rank = jsonUtils::extractValue<int>(info, "Rank", 0);

		m_pImageData = std::make_shared<Image::ImageData>();
		m_pImageData->setRank(m_rank);
		m_pImageData->setType(static_cast<Image::ImageType>(m_type));

		switch (m_type)
		{
			case Image::ImageType::Int16:   
				readImgArray<short>(info);  
				break;
			case Image::ImageType::Int32:   
				readImgArray<int>(info);    
				break;
			case Image::ImageType::Float64:
				readImgArray<double>(info);
				break;
			default: m_pImageData = nullptr;
		}

		m_clientTransactionID = jsonUtils::extractValue<unsigned int>(info, "ClientTransactionID", 0);
		m_serverTransactionID = jsonUtils::extractValue<unsigned int>(info, "ServerTransactionID", 0);
		m_errorNumber         = jsonUtils::extractValue<int>(info, "ErrorNumber", -1);
		m_errorMessage        = jsonUtils::extractValue<std::string>(info, "ErrorMessage", "");
	}
};

}
}


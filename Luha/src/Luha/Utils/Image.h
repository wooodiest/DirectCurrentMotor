#pragma once

#include "Luha/Core/Base.h"

namespace Luha {

	enum class ImageFilter
	{
		Nearest = 0, Linear
	};

	class Image
	{
	public:
		Image(const std::string& path, ImageFilter filter = ImageFilter::Linear);
		Image();
		~Image();

		Image(const Image& other);

		void* GetData();
		void LoadData(const std::string& path, ImageFilter filter);

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		float GetWidth() { return (float)m_Width; }
		float GetHeight() { return (float)m_Height; }

	private:
		uint32_t m_RendererID;
		int32_t m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
		uint32_t m_InternalFormat, m_DataFormat;
		ImageFilter m_ImageFilter = ImageFilter::Linear;

	};

}
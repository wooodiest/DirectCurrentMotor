#include "lhpch.h"
#include "Image.h"

#include "stb_image.h"

#include "glad/glad.h"

namespace Luha {

	Image::Image(const std::string& path, ImageFilter filter)
		: m_ImageFilter(filter)
	{
		LoadData(path, filter);
	
	}

	Image::Image()
	{
	}

	Image::~Image()
	{
		stbi_image_free(m_Data);

		glDeleteTextures(1, &m_RendererID);
	}

	void* Image::GetData()
	{
		glDeleteTextures(1, &m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_ImageFilter == ImageFilter::Nearest ? GL_NEAREST : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_ImageFilter == ImageFilter::Nearest ? GL_NEAREST : GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, m_Data);

		return (void*)m_RendererID; 
	}

	void Image::LoadData(const std::string& path, ImageFilter filter)
	{
		m_ImageFilter = filter;

		// stbi_set_flip_vertically_on_load(1);
		m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

		if (m_Channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (m_Channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
	}

	Image::Image(const Image& other)
	{
		m_RendererID = other.m_RendererID;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Channels = other.m_Channels;
		m_ImageFilter = other.m_ImageFilter;

		m_Data = new unsigned char[m_Width * m_Height * m_Channels];

		std::memcpy(m_Data, other.m_Data, m_Width * m_Height * m_Channels);
	}

}


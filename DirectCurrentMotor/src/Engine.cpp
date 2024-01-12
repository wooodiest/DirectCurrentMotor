#include "Engine.h"

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

namespace DCM {

	Engine::Engine()
	{
		SetViewportSize(1280.0f, 720.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float verticies[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

			-0.9f, -0.9f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.9f,  0.2f, 0.0f, 1.0f, 0.3f, 1.0f,
			-0.7f, -0.5f,  0.2f, 0.2f, 0.0f, 0.4f, 1.0f
		};

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_Shader = new Shader("assets/shaders/default.vert.glsl", "assets/shaders/default.frag.glsl");

	}

	Engine::~Engine()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);

		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);

		delete m_Shader;
	}

	void Engine::SetData(MotorSpecification& spec, Camera& camera)
	{
		m_Spec = &spec;
		m_Camera = &camera;
	}

	void Engine::OnUpdate(Luha::Timestep ts)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		glClearColor(0.29f, 0.314f, 0.322f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get Camera
		glm::mat4 viewProjection = m_Camera->GetViewProjection();		 
		m_Shader->Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });

		m_Shader->SetMat4("u_MVP", viewProjection * model);

		glBindVertexArray(m_VertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Engine::OnEvent(Luha::Event& e)
	{
		m_Camera->OnEvent(e);	
	}

	void Engine::SetViewportSize(float width, float height)
	{
		if (m_ViewportWidth != width || m_ViewportHeight != height) // Resize frame buffer
		{
			m_ViewportWidth  = width;
			m_ViewportHeight = height;

			if (m_RendererID)
			{
				glDeleteFramebuffers(1, &m_RendererID);
				glDeleteTextures(1, &m_ColorAttachment);
				glDeleteTextures(1, &m_DepthAttachment);
			}

			glCreateFramebuffers(1, &m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ViewportWidth, m_ViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_ViewportWidth, m_ViewportHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}	
	}

}
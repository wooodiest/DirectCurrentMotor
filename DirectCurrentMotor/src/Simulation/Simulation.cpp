#include "Simulation.h"

#include "glad/glad.h"
#include <OBJ_LOADER/OBJ_Loader.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace DCM {

	struct Vertex
	{
		glm::vec3 Position;
	};

	Simulation::Simulation()
	{
		SetViewportSize(1280.0f, 720.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_EngineShader = new Shader("assets/shaders/simulation.vert.glsl", "assets/shaders/simulation.frag.glsl");
		m_CoordShader = new Shader("assets/shaders/coord.vert.glsl", "assets/shaders/coord.frag.glsl");

		// Engine model
		{
			std::vector<float> coords, normals;
			std::vector<unsigned int> tris, solids;

			stl_reader::ReadStlFile("assets/models/engine.stl", coords, normals, tris, solids);
			int engineVertexCount = coords.size() / 3;
			m_EngineIndicesCount = tris.size();
			Vertex* engineFrame = new Vertex[engineVertexCount * sizeof(Vertex)];

			for (int i = 0; i < engineVertexCount; i++)
				engineFrame[i].Position = { coords[3 * i], coords[3 * i + 1], coords[3 * i + 2] };

			glGenVertexArrays(1, &m_EngineVertexArray);
			glBindVertexArray(m_EngineVertexArray);

			glGenBuffers(1, &m_EngineVertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_EngineVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, engineVertexCount * sizeof(engineFrame->Position), &engineFrame[0].Position, GL_STATIC_DRAW);

			glGenBuffers(1, &m_EngineIndexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EngineIndexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_EngineIndicesCount * sizeof(unsigned int), &tris[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}	
		// Coordinate system
		{
			glGenVertexArrays(1, &m_CoordVertexArray);
			glBindVertexArray(m_CoordVertexArray);

			float vertices[] =
			{
				-1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

				 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

				 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			};

			glGenBuffers(1, &m_CoordVertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_CoordVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

		}
	}

	Simulation::~Simulation()
	{
		// Frame buffer
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
		
		// Engine
		delete m_EngineShader;
		glDeleteBuffers(1, &m_EngineVertexBuffer);
		glDeleteBuffers(1, &m_EngineIndexBuffer);
		glDeleteVertexArrays(1, &m_EngineVertexArray);	

		// Coordinate system
		delete m_CoordShader;
		glDeleteBuffers(1, &m_CoordVertexBuffer);
		glDeleteVertexArrays(1, &m_CoordVertexArray);

	}

	void Simulation::SetData(MotorSpecification& spec, Camera& camera)
	{
		m_Spec = &spec;
		m_Camera = &camera;
	}

	void Simulation::OnUpdate(Luha::Timestep ts)
	{
		// Renderer commands
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get Camera
		glm::mat4 viewProjection = m_Camera->GetViewProjection();

		// Coordinate system
		if(m_ShowAxes)
		{
			const float scale = 100.0f;
			m_CoordShader->Bind();
			m_CoordShader->SetMat4("u_MVP", viewProjection 
				* glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f })
				* glm::scale(glm::mat4(1.0f), { scale, scale, scale }));

			glBindVertexArray(m_CoordVertexArray);
			glDrawArrays(GL_LINES, 0, 6);
		}
		// Engine
		{
			m_EngineShader->Bind();

			glm::mat4 model = 
				glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f }) *
				glm::toMat4(glm::quat(m_EngineInitialRotation + glm::vec3(0.0f, 0.0f, glm::radians(90.0f) - m_Spec->PrevAlpha))) *
				glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });

			m_EngineShader->SetMat4("u_MVP", viewProjection * model);

			glBindVertexArray(m_EngineVertexArray);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EngineIndexBuffer);
			glDrawElements(GL_TRIANGLES, m_EngineIndicesCount, GL_UNSIGNED_INT, nullptr);
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Simulation::OnEvent(Luha::Event& e)
	{
		m_Camera->OnEvent(e);	
	}

	void Simulation::SetViewportSize(float width, float height)
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

	void Simulation::DisplaySceneOptions()
	{
		ImGui::ColorEdit4("Background color", &m_ClearColor.r);
		ImGui::Checkbox("Show axes", &m_ShowAxes);

	}

}
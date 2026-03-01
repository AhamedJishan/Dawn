#include "Renderer.h"
#include "Utils/Log.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Assets.h"
#include "Rendering/Texture.h"
#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Core/Components/Camera.h"
#include "Core/Components/MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"

namespace Dawn
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init()
	{
		// Assumes a valid Opengl context is already initialised
		// which was done in Window::Init()

		int x, y;
		Application::Get()->GetWindow()->GetFrameBufferSize(x, y);
		glViewport(0, 0, x, y);

		Application::Get()->GetWindow()->SetFrameBufferSizeCallback([this](int width, int height) { glViewport(0, 0, width, height);});

		// Temporary==============================================
		InitQuad();

		return true;
	}
	
	void Renderer::Draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set Opengl state, like depth testing, etc.
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		for (MeshRenderer* meshRenderer : mMeshRenderers)
		{
			Camera* cam = meshRenderer->GetOwner()->GetScene()->GetActiveCamera();
			if (!cam)
			{
				LOG_WARN("No Active Camera Exists!");
				return;
			}

			glm::mat4 viewMatrix = cam->GetView();
			glm::mat4 projectionMatrix = cam->GetProjection();
			glm::mat4 modelMatrix = meshRenderer->GetOwner()->GetWorldTransform();

			Material* mat = meshRenderer->GetMaterial();
			Mesh* mesh = meshRenderer->GetMesh();
			Shader* shader = mat->GetShader();

			shader->Bind();
			mat->Apply();
			mesh->Bind();
			
			shader->SetMat4("u_Model", modelMatrix);
			shader->SetMat4("u_ViewProjection", projectionMatrix * viewMatrix);

			shader->SetVec3("u_CameraPosition", cam->GetOwner()->GetPosition());
			shader->SetVec3("u_AmbientColor", mLightingData.ambientColor);
			shader->SetVec3("u_DirectionalLightColor", mLightingData.directionalLight.color);
			shader->SetVec3("u_DirectionalLightDirection", mLightingData.directionalLight.direction);
			
			glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, NULL);
		}

		DrawQuad();
	}

	void Renderer::AddMeshRenderer(MeshRenderer* meshRenderer)
	{
		unsigned int updateOrder = meshRenderer->GetUpdateOrder();

		auto it = mMeshRenderers.begin();
		for (; it != mMeshRenderers.end(); it++)
			if ((*it)->GetUpdateOrder() > updateOrder)
				break;

		mMeshRenderers.insert(it, meshRenderer);
	}
	
	void Renderer::RemoveMeshRenderer(MeshRenderer* meshRenderer)
	{
		auto it = std::find(mMeshRenderers.begin(), mMeshRenderers.end(), meshRenderer);
		if (it != mMeshRenderers.end())
			mMeshRenderers.erase(it);
	}

	void Renderer::InitQuad()
	{
		float quadVertices[] =
		{
			// pos        // uv
			-0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 1.0f
		};
		unsigned int quadIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &mQuadVAO);
		glBindVertexArray(mQuadVAO);

		glGenBuffers(1, &mQuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glGenBuffers(1, &mQuadEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * (sizeof(float))));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer::DrawQuad()
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int x, y;
		Application::Get()->GetWindow()->GetFrameBufferSize(x, y);
		glm::mat4 orthoProjection = glm::ortho(0.0f, (float)x, 0.0f, (float)y);
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(x / 2, y / 2, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 1.0f));

		Shader* shader = Assets::GetShader("ui_unlit");
		Texture* texture = Assets::GetTexture("Assets/Textures/dot.png");

		shader->Bind();
		texture->Bind(0);
		shader->SetInt("u_texture", 0);
		shader->SetMat4("u_Model", model);
		shader->SetMat4("u_ViewProjection", orthoProjection);

		glBindVertexArray(mQuadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}
}
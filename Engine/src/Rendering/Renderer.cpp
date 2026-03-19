#include "Renderer.h"
#include "Utils/Log.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Core/Application.h"
#include "Core/Window.h"
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

		return true;
	}
	
	void Renderer::Draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set Opengl state, like depth testing, etc.
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		Camera* cam = Application::Get()->GetScene()->GetActiveCamera();
		if (!cam)
			return;

		glm::mat4 viewMatrix = cam->GetView();
		glm::mat4 projectionMatrix = cam->GetProjection();

		const EnvironmentSettings& environmentSettings = Application::Get()->GetScene()->GetEnvironmentSettings();

		for (MeshRenderer* meshRenderer : mMeshRenderers)
		{
			glm::mat4 modelMatrix = meshRenderer->GetOwner()->GetWorldTransform();

			Material* mat = meshRenderer->GetMaterial();
			Mesh* mesh = meshRenderer->GetMesh();
			Shader* shader = mat->GetShader();

			shader->Bind();
			mat->Apply();
			mesh->Bind();
			
			shader->SetMat4("u_Model", modelMatrix);
			shader->SetMat4("u_View", viewMatrix);
			shader->SetMat4("u_Projection", projectionMatrix);

			shader->SetFloat("u_FogDensity", environmentSettings.fogDensity);
			shader->SetVec3("u_FogColor", environmentSettings.fogColor);
			shader->SetVec3("u_CameraPosition", cam->GetOwner()->GetPosition());
			shader->SetVec3("u_AmbientColor", environmentSettings.ambientColor);
			shader->SetVec3("u_DirectionalLightColor", environmentSettings.directionalLight.color);
			shader->SetVec3("u_DirectionalLightDirection", environmentSettings.directionalLight.direction);
			
			glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, NULL);
		}
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
}
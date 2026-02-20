#include "Renderer.h"

#include <algorithm>
#include <glad/glad.h>
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Components/MeshRenderer.h"

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

		// TODO: Set Opengl state, like depth testing, etc.

		return true;
	}
	
	void Renderer::Draw()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Draw the scene
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
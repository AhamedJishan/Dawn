#include "MeshRenderer.h"

#include "Core/Actor.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Core/Application.h"
#include "Rendering/Renderer.h"

namespace Dawn
{
	MeshRenderer::MeshRenderer(Actor* owner, Mesh* mesh, Material* material, unsigned int updateOrder)
		:Component(owner, updateOrder)
		,mMesh(mesh)
		,mMaterial(material)
	{
		Application::Get()->GetRenderer()->AddMeshRenderer(this);
	}

	MeshRenderer::~MeshRenderer()
	{
		Application::Get()->GetRenderer()->RemoveMeshRenderer(this);

		delete mMaterial;
		// Mesh will be deleted by AssetManager
	}
}
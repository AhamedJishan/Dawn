#include "MeshRenderer.h"

#include "Core/Actor.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

namespace Dawn
{
	MeshRenderer::MeshRenderer(Actor* owner, Mesh* mesh, Material* material)
		:Component(owner)
		,mMesh(mesh)
		,mMaterial(material)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
		delete mMaterial;

		// Mesh will be deleted by AssetManager
	}
}
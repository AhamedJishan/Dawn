#pragma once

#include "Core/Component.h"

namespace Dawn
{
	// Forward declarations
	class Actor;
	class Mesh;
	class Material;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(Actor* owner, Mesh* mesh, Material* material);
		~MeshRenderer();

		Mesh* GetMesh() const { return mMesh; }

		Material* GetMaterial() const { return mMaterial; }
		void SetMaterial(Material* material) { delete mMaterial; mMaterial = material; }

	private:
		Mesh* mMesh;
		Material* mMaterial;
	};
}
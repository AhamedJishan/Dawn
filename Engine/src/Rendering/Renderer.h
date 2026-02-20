#pragma once

#include <vector>

namespace Dawn
{
	// Forward declaration
	class MeshRenderer;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Init();

		void Draw();

		// To be called by Constructor of MeshRenderer
		void AddMeshRenderer(MeshRenderer* meshRenderer);
		// To be called by Destructor of MeshRenderer
		void RemoveMeshRenderer(MeshRenderer* meshRenderer);

	private:
		std::vector<MeshRenderer*> mMeshRenderers;
	};
}
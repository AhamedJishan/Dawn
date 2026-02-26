#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward declaration
	class MeshRenderer;
	class Shader;

	struct LightingData
	{
		glm::vec3 ambientColor = glm::vec3(0.2f);
		
		struct DirectionalLight
		{
			glm::vec3 direction = glm::vec3(-1);
			glm::vec3 color = glm::vec3(1);
		} directionalLight;
	};

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
		LightingData mLightingData;

		//=====================================================
		// Temporary until UI system is not made
		//=====================================================
		void InitQuad();
		void DrawQuad();

		unsigned int mQuadVAO = 0, mQuadVBO = 0, mQuadEBO = 0;
	};
}
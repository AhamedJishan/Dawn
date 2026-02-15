#pragma once

#include <vector>
#include "Vertex.h"

namespace Dawn
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, int materialIndex);
		~Mesh();

		void Bind() const;

		unsigned int GetIndexCount() const { return mIndexCount; }
		int GetMaterialIndex() const { return mMaterialIndex; }

	private:
		void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	private:
		unsigned int mVAO = 0;
		unsigned int mVBO = 0;
		unsigned int mEBO = 0;

		unsigned int mIndexCount = 0;
		int mMaterialIndex = -1;
	};
}
#pragma once

#include <string>
#include <vector>
#include "Vertex.h"

namespace Dawn
{
	// CPU-side mesh data loaded from a model file (no GPU resources).
	class RawMesh
	{
	public:
		RawMesh(const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, int rawMaterialIndex);

		const std::string& GetName() const { return mName; }
		int GetRawMaterialIndex() const { return mRawMaterialIndex; }
		const std::vector<Vertex>& GetVertices() const { return mVertices; }
		const std::vector<unsigned int>& GetIndices() const { return mIndices; }

	private:
		std::string mName;
		int mRawMaterialIndex;

		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;
	};
}
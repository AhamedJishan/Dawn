#include "RawMesh.h"

#include <utility>

namespace Dawn
{
	RawMesh::RawMesh(const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, int rawMaterialIndex)
		:mName(name)
		,mRawMaterialIndex(rawMaterialIndex)
		,mVertices(std::move(vertices))
		,mIndices(std::move(indices))
	{
	}
}
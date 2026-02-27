#pragma once

#include <glm/vec3.hpp>

namespace Dawn
{
	struct Ray
	{
		glm::vec3 origin = glm::vec3(0);
		glm::vec3 direction = glm::vec3(0, 0, -1);;
	};

	struct Sphere
	{
		glm::vec3 center = glm::vec3(0);
		float radius = 1.0f;
	};
}
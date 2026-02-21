#include "Camera.h"

#include "Core/Actor.h"
#include <Core/Application.h>
#include <Core/Window.h>

namespace Dawn
{
	Camera::Camera(Actor* owner, unsigned int updateOrder)
		:Component(owner, updateOrder)
	{
	}

	glm::mat4 Camera::GetView() const
	{
		glm::vec3 position = mOwner->GetPosition();
		glm::vec3 forward  = mOwner->GetForward();
		glm::vec3 up = mOwner->GetUp();

		return glm::lookAt(position, position + forward, up);
	}
	
	glm::mat4 Camera::GetProjection() const
	{
		int width = Application::Get()->GetWindow()->GetWidth();
		int height = Application::Get()->GetWindow()->GetHeight();

		if (height == 0) height = 1;
		float aspect = static_cast<float>(width) / static_cast<float>(height);

		return glm::perspective(glm::radians(mFOV), aspect, mNear, mFar);
	}
}
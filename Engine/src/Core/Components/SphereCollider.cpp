#include "SphereCollider.h"

#include "Core/Actor.h"
#include "Physics/Physics.h"

namespace Dawn
{
	SphereCollider::SphereCollider(Actor* owner, unsigned int updateOrder)
		:Component(owner, updateOrder)
	{
	}

	Sphere SphereCollider::GetWorldSphere() const
	{
		Sphere sphere;
		sphere.center = mOwner->GetPosition() + mOffset;
		sphere.radius = mRadius;

		return sphere;
	}
}
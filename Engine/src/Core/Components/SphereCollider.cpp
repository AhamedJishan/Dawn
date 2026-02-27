#include "SphereCollider.h"

#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Physics/Physics.h"

namespace Dawn
{
	SphereCollider::SphereCollider(Actor* owner, unsigned int updateOrder)
		:Component(owner, updateOrder)
	{
		mOwner->GetScene()->AddSphereCollider(this);
	}

	SphereCollider::~SphereCollider()
	{
		mOwner->GetScene()->RemoveSphereCollider(this);
	}

	Sphere SphereCollider::GetWorldSphere() const
	{
		Sphere sphere;
		sphere.center = mOwner->GetPosition() + mOffset;
		sphere.radius = mRadius;

		return sphere;
	}
}
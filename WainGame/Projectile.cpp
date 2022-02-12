#include "Projectile.h"
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec\Texture.h>

Projectile::Projectile(std::string texture_path, Transform transform, PhysicsProperties properties) 
	: Rigidbody(texture_path, transform, properties)
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
	mCollider->IsOverlap = true;
}

Projectile::~Projectile()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}
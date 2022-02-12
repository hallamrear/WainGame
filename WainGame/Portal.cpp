#include "Portal.h"
#include "Player.h"
#include <HalTec\Texture.h>
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec/StateDirector.h>

Portal::Portal(std::string texture, Transform transform)
	: Rigidbody(texture, transform, PhysicsProperties(0.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
	mCollider->IsOverlap = true;
}

Portal::~Portal()
{

}

void Portal::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{
	if (dynamic_cast<Player*>(&other))
	{
		mIsAlive = false;
		other.SetAlive(false);
	}
}

void Portal::Update(double DeltaTime)
{
	mCollider->Update(DeltaTime);
}

void Portal::Render()
{
	mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
	mCollider->Render(mRenderer);
}

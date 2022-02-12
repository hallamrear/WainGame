#include "WorldPiece.h"
#include "Player.h"
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec\Texture.h>

WorldPiece::WorldPiece(std::string texture_path, Transform transform)
	: Rigidbody(texture_path, transform, PhysicsProperties(1.0f, 0.5f, 20000.0f, 0.1f, false, false, true))
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

WorldPiece::~WorldPiece()
{

}

void WorldPiece::Update(double DeltaTime)
{
	mCollider->Update(DeltaTime);
}

void WorldPiece::Render()
{
	mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
	mCollider->Render(mRenderer);
}

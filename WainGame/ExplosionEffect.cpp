#include "ExplosionEffect.h"
#include <HalTec\Animation.h>

ExplosionEffect::ExplosionEffect(Vector2f position)
	: Rigidbody("", Transform(position), PhysicsProperties(0.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	mAnimation = new AnimationController("Textures/Explosion (1).bmp", 1, 8, 1.0f, false);
}

ExplosionEffect::~ExplosionEffect()
{
	delete mAnimation;
	mAnimation = nullptr;
}

void ExplosionEffect::Update(double DeltaTime)
{
	if (mIsAlive)
	{
		if (mAnimation->HasFinished())
		{
			SetAlive(false);
		}
		else
		{
			mAnimation->Update(DeltaTime);
		}
	}
}

void ExplosionEffect::Render()
{
	if (mIsAlive)
	{
		mAnimation->Render(mRenderer, mTransform.Position);
	}
}

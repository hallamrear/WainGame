#include "ShootCircleAttackPhase.h"
#include "Boss.h"
#include <HalTec\Texture.h>
#include <HalTec\Rigidbody.h>

ShootCircleAttackPhase::ShootCircleAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path)
	: AttackPhase(renderer, boss, projectiles, texture_path)
{
	mShootCooldown = 1.5f;
	mUseAttackTexture = true;

	mAttackTextureCooldown = 0.25f;
}

ShootCircleAttackPhase::~ShootCircleAttackPhase()
{

}

void ShootCircleAttackPhase::Update(double DeltaTime)
{
	mShootTimer += DeltaTime;

	if (mUseAttackTexture)
	{
		mAttackTextureTimer += DeltaTime;

		if (mAttackTextureTimer > mAttackTextureCooldown)
		{
			mUseAttackTexture = false;
			mAttackTextureTimer = 0.0f;
		}
	}

	if (mShootTimer > mShootCooldown)
	{
		mBoss.FireBulletsInCircle();
		mUseAttackTexture = true;
		mShootTimer = 0.0f;
	}
}

void ShootCircleAttackPhase::Render()
{
	mAttackTexture->Render(mRenderer, mBoss.GetTransform().Position, mBoss.GetTransform().Rotation);
}

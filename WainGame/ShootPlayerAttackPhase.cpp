#include "ShootPlayerAttackPhase.h"
#include "Boss.h"
#include <HalTec\Texture.h>
#include <HalTec\Rigidbody.h>

ShootPlayerAttackPhase::ShootPlayerAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path)
	: AttackPhase(renderer, boss, projectiles, texture_path)
{
	mShootCooldown = 1.0f;
	mUseAttackTexture = true;

	mAttackTextureCooldown = 0.25f;
}

ShootPlayerAttackPhase::~ShootPlayerAttackPhase()
{

}

void ShootPlayerAttackPhase::Update(double DeltaTime)
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
		mBoss.FireBulletAtPlayer();	
		mUseAttackTexture = true;
		mShootTimer = 0.0f;
	}
}

void ShootPlayerAttackPhase::Render()
{
	mAttackTexture->Render(mRenderer, mBoss.GetTransform().Position, mBoss.GetTransform().Rotation);
}

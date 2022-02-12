#include "ShootBeamAttackPhase.h"
#include "Boss.h"
#include <HalTec\Texture.h>
#include <HalTec\Rigidbody.h>

ShootBeamAttackPhase::ShootBeamAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path)
	: AttackPhase(renderer, boss, projectiles, texture_path)
{
	mShootTimer = 0.0f;
	mShootCooldown = 8.0f;
	mUseAttackTexture = false;

	mAttackTextureCooldown = 4.0f;
}

ShootBeamAttackPhase::~ShootBeamAttackPhase()
{

}

void ShootBeamAttackPhase::Update(double DeltaTime)
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
	else
	{
		if (mShootTimer > mShootCooldown)
		{
			mBoss.FireBeams();
			mUseAttackTexture = true;
			mShootTimer = 0.0f;
		}
	}
}

void ShootBeamAttackPhase::Render()
{
	mAttackTexture->Render(mRenderer, mBoss.GetTransform().Position, mBoss.GetTransform().Rotation);
}

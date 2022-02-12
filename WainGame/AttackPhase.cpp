#include "AttackPhase.h"
#include <HalTec\TextureCache.h>
#include <HalTec\Texture.h>

bool AttackPhase::GetUseAttackTexture()
{
	return mUseAttackTexture;
}

void AttackPhase::SetUseAttackTexture(bool state)
{
	mUseAttackTexture = state;
}

AttackPhase::AttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path) : mBoss(boss), mProjectiles(projectiles), mRenderer(renderer)
{
	mUseAttackTexture = false;

	if(texture_path != "")
		mAttackTexture = TextureCache::GetTexture(texture_path);
}

AttackPhase::~AttackPhase()
{

}

void AttackPhase::Reset()
{
	mShootTimer = 0.0f;
}

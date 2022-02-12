#pragma once
#include <vector>
#include <string>
#include <HalTec\Entity.h>

class Boss;
class Rigidbody;

class AttackPhase
{
protected:
	float mShootTimer;
	float mShootCooldown;

	std::vector<Rigidbody*>& mProjectiles;
	Boss& mBoss;
	SDL_Renderer& mRenderer;
	bool mUseAttackTexture;
	float mAttackTextureCooldown;
	float mAttackTextureTimer;
	class Texture* mAttackTexture;

public:
	bool GetUseAttackTexture();
	void SetUseAttackTexture(bool state);

	AttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path);
	virtual ~AttackPhase() = 0;

	void Reset();

	virtual void Update(double DeltaTime) = 0;
	virtual void Render() = 0;
};


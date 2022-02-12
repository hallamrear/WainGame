#pragma once
#include "Projectile.h"

class BossBeam : public Projectile
{
private:
	float mMaxLifespan;
	float mLifespan;

public:
	BossBeam(std::string texture_path, Transform transform);
	~BossBeam();

	void OnCollision(const CollisionManifold&, Rigidbody& other);

	void Update(double DeltaTime);
	void Render();
};


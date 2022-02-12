#pragma once
#include "Projectile.h"

class BossProjectile : public Projectile
{
public:
	BossProjectile(std::string texture_path, Transform transform);
	~BossProjectile();

	void OnOverlap(const CollisionManifold&, Rigidbody& other);

	void Update(double DeltaTime);
	void Render();
};


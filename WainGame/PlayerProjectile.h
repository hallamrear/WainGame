#pragma once
#include "Projectile.h"
class PlayerProjectile :
    public Projectile
{
public:
	PlayerProjectile(std::string texture_path, Transform transform);
	~PlayerProjectile();

	void OnOverlap(const CollisionManifold&, Rigidbody& other);

	void Update(double DeltaTime);
	void Render();
};


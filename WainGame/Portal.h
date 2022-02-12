#pragma once
#include <HalTec\Rigidbody.h>

class Portal : public Rigidbody
{
public:
	Portal(std::string texture, Transform transform);
	~Portal();

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& other);

	void Update(double DeltaTime);
	void Render();
};


#pragma once
#include <HalTec\Rigidbody.h>

class ExplosionEffect : public Rigidbody
{
private:
	class AnimationController* mAnimation;

public:
	ExplosionEffect(Vector2f position);
	~ExplosionEffect();

	void Update(double DeltaTime);
	void Render();
};


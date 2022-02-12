#pragma once
#include <HalTec\Rigidbody.h>

class Projectile :
    public Rigidbody
{
public:
    Projectile(std::string texture_path, Transform transform, PhysicsProperties properties);
    virtual ~Projectile() = 0;

    virtual void Update(double) = 0;
    virtual void Render() = 0;
};


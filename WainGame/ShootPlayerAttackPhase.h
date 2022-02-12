#pragma once
#include "AttackPhase.h"

class ShootPlayerAttackPhase :
    public AttackPhase
{
private:

public:
    ShootPlayerAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path);
    ~ShootPlayerAttackPhase();

    void Update(double DeltaTime);
    void Render();
};


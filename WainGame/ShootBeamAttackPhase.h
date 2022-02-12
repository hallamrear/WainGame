#pragma once
#include "AttackPhase.h"

class ShootBeamAttackPhase :
    public AttackPhase
{
private:

public:
    ShootBeamAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path);
    ~ShootBeamAttackPhase();

    void Update(double DeltaTime);
    void Render();
};
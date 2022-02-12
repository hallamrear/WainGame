#pragma once
#include "AttackPhase.h"
class ShootCircleAttackPhase :
    public AttackPhase
{
private:

public:
    ShootCircleAttackPhase(SDL_Renderer& renderer, Boss& boss, std::vector<Rigidbody*>& projectiles, std::string texture_path);
    ~ShootCircleAttackPhase();

    void Update(double DeltaTime);
    void Render();
};
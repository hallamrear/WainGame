#pragma once
#include <HalTec\Rigidbody.h>

class Texture;

class Player :
    public Rigidbody
{
private:
    std::vector<Rigidbody*>& mRigidbodies;
    class AnimationController* mAnimation;
    Texture* mReticuleTexture;
    Texture* mGrave;
    Transform mReticuleTransform;
    float mReticuleDistance;
    float mMovementSpeed;
    float mShootTimer;
    float mShootCooldown;
    bool mCanShoot;
    float mJumpForce;

    void MoveLeftRight(float value);
    void Fire();

public:
    Player(std::vector<Rigidbody*>& rigidbodies, Transform transform);
    ~Player();

    void Jump();

    void Update(double);
    void Render();
};


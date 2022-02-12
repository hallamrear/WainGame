#pragma once
#include <HalTec\Rigidbody.h>
#include <vector>


class OrientedBoundingBox;
class BoundingBox;
class AnimationController;
class AttackPhase;
class Projectile;
class Player;

class Boss :
    public Rigidbody
{
private:
	Player& mPlayer;
	AnimationController* mHealthBarAnimation;
	BoundingBox* mTargetPoint;
	OrientedBoundingBox* mPointChecker;
	std::vector<Vector2f> mPathPoints;
	Vector2f mCurrentPathTargetPosition;
	int mCurrentPathTargetIndex;
	float mMoveSpeedScale;

	int mMaxHealth;
	int mHealth;
	AnimationController* mDeathAnimation;
	std::vector<AnimationController*> mDeathExplosions;
	std::vector<AttackPhase*> mAttackPhases;
	std::vector<Vector2f> mDeathExplosionOffsets;
	std::vector<Rigidbody*>& mEntities;
	float mExplosionSoundEffectElapsedTime;
	float mExplosionSoundEffectCooldown;
	int mCurrentAttackPhase;

	float mIsStillCooldown;
	float mIsStillTimer;
	bool mIsStill;
	float mChangeAttackTimer;
	float mChangeAttackCooldown;

public:

	Boss(std::string texture_path, Transform transform, std::vector<Rigidbody*>& worldEntities, Player& player);
	~Boss();

	virtual void OnOverlap(const CollisionManifold& manifold, Rigidbody& other);

	void FireBulletAtPlayer();
	void FireBulletsInCircle();
	void FireBeams();

	void AddPathPoint(Vector2f point);

	virtual void			Update(double deltaTime);
	virtual void			Render();
};


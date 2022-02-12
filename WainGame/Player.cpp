#include "Player.h"
#include <HalTec\Animation.h>
#include <HalTec\TextureCache.h>
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec\InputManager.h>
#include "PlayerProjectile.h"
#include <HalTec\MyTime.h>

Player::Player(std::vector<Rigidbody*>& rigidbodies, Transform transform)
	: Rigidbody("", transform, PhysicsProperties(5.0f, 0.1f, 10.0f, 1.0f, true, true, false)), mRigidbodies(rigidbodies)
{
	mAnimation = new AnimationController("Textures/Player.bmp", 6, 8, 1.0f, true);
	mAnimation->SetAnimation(1);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 20.0f, mAnimation->FrameSize.Y - 5.0f);
	mReticuleTexture = TextureCache::GetTexture("Textures/Reticule.bmp");
	mGrave = TextureCache::GetTexture("Textures/Grave.bmp");

	mReticuleDistance = 20.0f;
	mShootCooldown = 0.7f;

	InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, std::bind(&Player::MoveLeftRight, this, 1.0f));
	InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, std::bind(&Player::MoveLeftRight, this, -1.0f));
	InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_RELEASED, [this]() { mAnimation->SetAnimation(1); } );
	InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_RELEASED, [this]() { mAnimation->SetAnimation(1); } );	
	InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Player::Jump, this));

	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Player::Fire, this));
	mMovementSpeed = 10.0f;
	mJumpForce = 1000.0f;
}

Player::~Player()
{
	
}

void Player::Jump()
{
	if(mVelocity.Y < 0.3f && mVelocity.Y > -0.3f && mIsAlive)	
		AddVelocity(Vector2f(0.0f, mJumpForce * Time::DeltaTime()));
}

void Player::MoveLeftRight(float value)
{
	if (mIsAlive)
	{
		if (mVelocity.X > 0.0f)
		{
			if (value < 0)
				mVelocity.X = 0.0f;
		}
		else
		{
			if (value > 0)
				mVelocity.X = 0.0f;
		}

		if (mAnimation->GetCurrentAnimationIndex() == 1)
		{
			if (value > 0)
				mAnimation->SetAnimation(0);
			else
				mAnimation->SetAnimation(2);
		}

		AddVelocity(Vector2f(value * mMovementSpeed * (float)Time::DeltaTime(), 0.0f));
	}
}

void Player::Fire()
{
	if (mCanShoot)
	{
		Vector2f pos = InputManager::Get()->GetMouseWorldPosition();
		mRigidbodies.push_back(new PlayerProjectile("Textures/PlayerProjectile.bmp", GetTransform()));
		Vector2f dir = pos - GetTransform().Position;
		mRigidbodies.back()->AddVelocity(dir.GetNormalized() * 10.0f);
		mRigidbodies.back()->GetTransform().Rotation = ConvertToDegrees(atan(dir.X / dir.Y));

		mCanShoot = false;
	}
}

void Player::Update(double DeltaTime)
{
	mVelocity.Y = std::min(mVelocity.Y, 9.81f);

	mCollider->Update(DeltaTime);

	if (mIsAlive)
	{
		if (!mCanShoot)
		{
			mShootTimer += DeltaTime;

			if (mShootTimer > mShootCooldown)
			{
				mCanShoot = true;
				mShootTimer = 0.0f;
			}
		}

		mAnimation->Update(DeltaTime);

		Vector2f dir = (InputManager::Get()->GetMouseWorldPosition() - mTransform.Position).GetNormalized();
		mReticuleTransform.Position = mTransform.Position + (dir * mReticuleDistance);
		mReticuleTransform.Rotation = ConvertToDegrees(atan(dir.X / dir.Y));
	}
	else
	{
		mGravityEnabled = false;
		mDragEnabled = false;
		mVelocity = Vector2f();
	}
}

void Player::Render()
{
	if (mIsAlive)
	{
		mAnimation->Render(mRenderer, mTransform);
		mReticuleTexture->Render(mRenderer, mReticuleTransform.Position, mReticuleTransform.Rotation);
	}
	else
	{
		mGrave->Render(mRenderer, mTransform.Position, mTransform.Rotation);
	}

	mCollider->Render(mRenderer);
}

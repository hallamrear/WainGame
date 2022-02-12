#pragma once
#include <HalTec\Rigidbody.h>

class OrientedBoundingBox;

class WorldPiece : public Rigidbody
{
private:

public:
	WorldPiece(std::string texture_path, Transform transform);
	~WorldPiece();

	void Update(double DeltaTime);
	void Render();
};


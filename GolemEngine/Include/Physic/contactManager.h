#pragma once

#include <mutex>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>


using namespace JPH;

class Collider;

enum class ColliderState
{
	DEFAULT,
	ON_COLLISION_ENTER,
	ON_COLLISION_STAY,
	ON_COLLISION_EXIT
};

struct CollisionData
{
	Collider* collider1;
	Collider* collider2;
	ColliderState colliderState;
};

class ContactManager : public ContactListener
{
private:
	std::mutex m_mutex;
	std::vector<CollisionData> m_collisions;

public:
	virtual ValidateResult OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult) override;

	virtual void OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override;
	
	virtual void OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override;

	virtual void OnContactRemoved(const SubShapeIDPair& inSubShapePair) override;

	virtual void UpdateAllContacts();
};
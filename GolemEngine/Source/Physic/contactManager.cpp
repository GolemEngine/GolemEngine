#include "Physic/contactManager.h"
#include "Physic/physicSystem.h"
#include "Components/Physic/collider.h"
#include "Core/gameobject.h"

#include <iostream>

ValidateResult ContactManager::OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult)
{
	// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
	return ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactManager::OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings)
{
	Collider* c1 = PhysicSystem::GetCollider(inBody1.GetID());
	Collider* c2 = PhysicSystem::GetCollider(inBody2.GetID());
	CollisionData collisionData = {
		.collider1 = c1,
		.collider2 = c2,
		.colliderState = ColliderState::ON_COLLISION_ENTER
	};
	
	{
		std::scoped_lock(m_mutex);
		m_collisions.push_back(collisionData); 
	}
}

void ContactManager::OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings)
{
	Collider* c1 = PhysicSystem::GetCollider(inBody1.GetID());
	Collider* c2 = PhysicSystem::GetCollider(inBody2.GetID());
	CollisionData collisionData = {
		.collider1 = c1,
		.collider2 = c2,
		.colliderState = ColliderState::ON_COLLISION_STAY
	};
	{
		std::scoped_lock(m_mutex);
		m_collisions.push_back(collisionData);
	}
}

void ContactManager::OnContactRemoved(const SubShapeIDPair& inSubShapePair)
{
}

void ContactManager::UpdateAllContacts()
{
	for (CollisionData collisionData : m_collisions)
	{
		switch (collisionData.colliderState)
		{
		case ColliderState::DEFAULT:
			break;
		case ColliderState::ON_COLLISION_ENTER:
			collisionData.collider1->OnCollisionEnter(collisionData.collider2);
			collisionData.collider2->OnCollisionEnter(collisionData.collider1);
			break;
		case ColliderState::ON_COLLISION_STAY:
			collisionData.collider1->OnCollisionStay(collisionData.collider2);
			collisionData.collider2->OnCollisionStay(collisionData.collider1);
			break;
		case ColliderState::ON_COLLISION_EXIT:
			break;
		default:
			break;
		}
	}
	m_collisions.clear();
}

#include "Physic/physicSystem.h"

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

// STL includes
#include <iostream>
#include <cstdarg>
#include <thread>

#include "Components/Physic/sphereCollider.h"


// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

using namespace JPH;
using namespace JPH::literals;
using namespace std;

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char* inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	cout << buffer << endl;
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
{
	// Print to the TTY
	cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

	// Breakpoint
	return true;
};

#endif // JPH_ENABLE_ASSERTS

RVec3 PhysicSystem::ToJph(const Vector3 _v)
{
	return RVec3(_v.x, _v.y, _v.z);
}

Quat PhysicSystem::ToJph(const Quaternion _q)
{
	return Quat(_q.x, _q.y, _q.z, _q.w);
}

PhysicSystem::PhysicSystem()
{
	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	RegisterDefaultAllocator();

	// Install trace and assert callbacks
	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();

	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	const uint cMaxBodies = 65536;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint cNumBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	const uint cMaxBodyPairs = 65536;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	const uint cMaxContactConstraints = 10240;

	// Now we can create the actual physics system.
	physicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broadPhaseLayerInterface, objectVsBroadphaseLayerFilter, objectVsObjectLayerFilter);

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	physicsSystem.SetContactListener(&contactListener);

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	physicsSystem.OptimizeBroadPhase();
}

PhysicSystem::~PhysicSystem()
{
	// Unregisters all types with the factory and cleans up the default material
	UnregisterTypes();

	// Destroy the factory
	delete Factory::sInstance;
	Factory::sInstance = nullptr;
}

Vector3 PhysicSystem::ToVector3(const RVec3 _v)
{
	return Vector3(_v.GetX(), _v.GetY(), _v.GetZ());
}

Quaternion PhysicSystem::ToQuaternion(const Quat _q)
{
	return Quaternion(_q.GetW(), _q.GetX(), _q.GetY(), _q.GetZ());
}

void PhysicSystem::PreUpdate()
{
	for (Collider* c : m_colliders)
	{
		c->PreUpdate();
	}
}

void PhysicSystem::Update()
{
	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
	// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
	// malloc / free.
	TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

	contactListener.UpdateAllContacts();

	physicsSystem.Update(cDeltaTime, cCollisionSteps, &temp_allocator, &job_system);
}


void PhysicSystem::PostUpdate()
{
	for (Collider* c : m_colliders)
	{
		c->PostUpdate();
	}
}

void PhysicSystem::AddCollider(Collider* _collider)
{
	m_colliders.push_back(_collider);
}

Collider* PhysicSystem::GetCollider(BodyID _bodyId)
{
	for (Collider* c : m_colliders)
	{
		if (c->id == _bodyId)
			return c;
	}
	return nullptr;
}

void PhysicSystem::DeleteCollider(Collider* _collider)
{
	std::erase(m_colliders, _collider);
}

BodyID PhysicSystem::CreateSphereCollider(Vector3 _position, Quaternion _rotation, float _radius, EMotionType _motionType, ObjectLayer _objectLayer)
{
	 BodyInterface& body_interface = PhysicSystem::physicsSystem.GetBodyInterface();

	 BodyCreationSettings sphere_settings(new SphereShape(_radius), ToJph(_position), ToJph(_rotation), _motionType, _objectLayer);
	 sphere_settings.mAllowDynamicOrKinematic = true;
	 return body_interface.CreateAndAddBody(sphere_settings, EActivation::DontActivate);
}

BodyID PhysicSystem::CreateBoxCollider(Vector3 _position, Quaternion _rotation, Vector3 _size, EMotionType _motionType, ObjectLayer _objectLayer)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();

	BodyCreationSettings boxSettings(new BoxShape(ToJph(_size)), ToJph(_position), ToJph(_rotation), _motionType, _objectLayer);
	boxSettings.mAllowDynamicOrKinematic = true;
	return bodyInterface.CreateAndAddBody(boxSettings, EActivation::DontActivate);
}

BodyID PhysicSystem::CreateCapsuleCollider(Vector3 _position, Quaternion _rotation, float _height, float _radius, EMotionType _motionType, ObjectLayer _objectLayer)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();

	BodyCreationSettings boxSettings(new CapsuleShape(_height, _radius), ToJph(_position), ToJph(_rotation), _motionType, _objectLayer);
	boxSettings.mAllowDynamicOrKinematic = true;
	return bodyInterface.CreateAndAddBody(boxSettings, EActivation::DontActivate);
}
BodyID PhysicSystem::CreateMeshCollider(const std::vector<Vertex>& _vertices, Vector3 _position, Quaternion _rotation, EMotionType _motionType, ObjectLayer _objectLayer)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();

	std::vector<Vec3, STLAllocator<Vec3>> vecPoints;

	for (Vertex v : _vertices)
	{
		vecPoints.push_back(Vec3(v.position.x, v.position.y, v.position.z));
	}
	Array<Vec3> points(vecPoints);

	ConvexHullShapeSettings convexHullSettings(points);
	ShapeSettings::ShapeResult shapeResult;
	ConvexHullShape* convex = new ConvexHullShape(convexHullSettings, shapeResult);
	BodyCreationSettings meshSettings(convex, ToJph(_position), ToJph(_rotation), _motionType, _objectLayer);
	meshSettings.mAllowDynamicOrKinematic = true;
	return bodyInterface.CreateAndAddBody(meshSettings, EActivation::DontActivate);
}

void PhysicSystem::MakeBodyStatic(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetMotionType(_bodyId, EMotionType::Static, EActivation::DontActivate);
	bodyInterface.SetObjectLayer(_bodyId, ObjectLayers::NON_MOVING);
}

void PhysicSystem::MakeBodyDynamic(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetMotionType(_bodyId, EMotionType::Dynamic, EActivation::DontActivate);
	bodyInterface.SetObjectLayer(_bodyId, ObjectLayers::MOVING);
}

void PhysicSystem::MakeBodyKinematic(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetMotionType(_bodyId, EMotionType::Kinematic, EActivation::DontActivate);
	bodyInterface.SetObjectLayer(_bodyId, ObjectLayers::MOVING);
}

void PhysicSystem::ActivateBody(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.ActivateBody(_bodyId);
}
void PhysicSystem::DeactivateBody(BodyID _bodyId, Vector3 _position)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.DeactivateBody(_bodyId);
	bodyInterface.SetPosition(_bodyId, ToJph(_position), EActivation::DontActivate);
}

void PhysicSystem::SetPosition(BodyID _bodyId, Vector3 _position)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetPosition(_bodyId, ToJph(_position), EActivation::DontActivate);
}

void PhysicSystem::UpdatePosition(BodyID _bodyId, Vector3& _position)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	RVec3 position = bodyInterface.GetPosition(_bodyId);
	_position = PhysicSystem::ToVector3(position);
}

void PhysicSystem::SetRotation(BodyID _bodyId, Vector3 _rotation)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetRotation(_bodyId, ToJph(Quaternion::EulerToQuaternion(_rotation)), EActivation::DontActivate);
}

void PhysicSystem::SetVelocity (BodyID _bodyId, const Vector3 _velocity)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetLinearVelocity(_bodyId, ToJph(_velocity));
}

void PhysicSystem::SetGravityFactor(BodyID _bodyId, float _gravityFactor)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetGravityFactor(_bodyId, _gravityFactor);
}

Vector3 PhysicSystem::GetVelocity(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	return ToVector3(bodyInterface.GetLinearVelocity(_bodyId));
}


void PhysicSystem::AddVelocity(BodyID _bodyId, Vector3 _velocity)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.AddLinearVelocity(_bodyId, ToJph(_velocity));
}

void PhysicSystem::AddForce(BodyID _bodyId, Vector3 _force, float _power)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.AddForce(_bodyId, ToJph(_force * _power * m_cDecentForceFactor));
}

void PhysicSystem::SetSphereShape(BodyID _bodyId, float _radius)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetShape(_bodyId, new SphereShape(_radius), false, EActivation::Activate);
}

void PhysicSystem::SetBoxShape(BodyID _bodyId, Vector3 _size)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetShape(_bodyId, new BoxShape(ToJph(_size)), false, EActivation::Activate);
}

void PhysicSystem::SetCapsuleShape(BodyID _bodyId, float _height, float _radius)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.SetShape(_bodyId, new CapsuleShape(_height, _radius), false, EActivation::Activate);
}

void PhysicSystem::SetConvexHullShape(BodyID _bodyId, std::vector<Vertex>& _vertices)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();

	std::vector<Vec3, STLAllocator<Vec3>> vecPoints;

	for (Vertex v : _vertices)
	{
		vecPoints.push_back(Vec3(v.position.x, v.position.y, v.position.z));
	}
	Array<Vec3> points(vecPoints);

	ConvexHullShapeSettings convexHullSettings(points);
	ShapeSettings::ShapeResult shapeResult;
	ConvexHullShape* convex = new ConvexHullShape(convexHullSettings, shapeResult);
	bodyInterface.SetShape(_bodyId, convex, false, EActivation::Activate);
}

void PhysicSystem::DeleteBody(BodyID _bodyId)
{
	BodyInterface& bodyInterface = PhysicSystem::physicsSystem.GetBodyInterface();
	bodyInterface.RemoveBody(_bodyId);

	// Destroy the body. After this the sphere ID is no longer valid.
	bodyInterface.DestroyBody(_bodyId);
}
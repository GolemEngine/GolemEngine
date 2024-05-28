#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

using namespace JPH;

class BodyActivationManager : public BodyActivationListener
{
public:
	virtual void OnBodyActivated(const BodyID& inBodyID, uint64 inBodyUserData) override;

	virtual void OnBodyDeactivated(const BodyID& inBodyID, uint64 inBodyUserData) override;
};
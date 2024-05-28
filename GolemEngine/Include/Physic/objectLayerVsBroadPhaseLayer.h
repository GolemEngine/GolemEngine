#pragma once

#include "Physic/layers.h"

using namespace JPH;

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectLayerVsBroadPhaseLayer : public ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool ShouldCollide(ObjectLayer _inLayer1, BroadPhaseLayer _inLayer2) const override;
};
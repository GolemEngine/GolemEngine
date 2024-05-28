#pragma once

#include "Physic/layers.h"

using namespace JPH;

class ObjectLayerVsObjectLayer : public ObjectLayerPairFilter
{
public:
	virtual bool ShouldCollide(ObjectLayer _inObject1, ObjectLayer _inObject2) const override;
};
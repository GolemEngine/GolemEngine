#include "Physic/objectLayerVsObjectLayer.h"
#include <iostream>

bool ObjectLayerVsObjectLayer::ShouldCollide(ObjectLayer _inObject1, ObjectLayer _inObject2) const
{
	switch (_inObject1)
	{
	case ObjectLayers::NON_MOVING:
		return _inObject2 == ObjectLayers::MOVING; // Non moving only collides with moving
	case ObjectLayers::MOVING:
		return true; // Moving collides with everything
	default:
		JPH_ASSERT(false);
		return false;
	}
}
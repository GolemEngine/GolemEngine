#include "Physic/objectLayerVsBroadPhaseLayer.h"

bool ObjectLayerVsBroadPhaseLayer::ShouldCollide(ObjectLayer _inLayer1, BroadPhaseLayer _inLayer2) const 
{
	switch (_inLayer1)
	{
	case ObjectLayers::NON_MOVING:
		return _inLayer2 == BroadPhaseLayers::MOVING;
	case ObjectLayers::MOVING:
		return true;
	default:
		JPH_ASSERT(false);
		return false;
	}
}
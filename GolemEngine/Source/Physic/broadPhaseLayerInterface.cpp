#include "Physic/broadPhaseLayerInterface.h"

BPLInterface::BPLInterface()
{
	// Create a mapping table from object to broad phase layer
	m_ObjectToBroadPhase[ObjectLayers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
	m_ObjectToBroadPhase[ObjectLayers::MOVING] = BroadPhaseLayers::MOVING;
}

uint BPLInterface::GetNumBroadPhaseLayers() const
{
	return BroadPhaseLayers::NUM_LAYERS;
}

BroadPhaseLayer BPLInterface::GetBroadPhaseLayer(ObjectLayer _inLayer) const
{
	JPH_ASSERT(_inLayer < ObjectLayers::NUM_LAYERS);
	return m_ObjectToBroadPhase[_inLayer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* BPLInterface:: GetBroadPhaseLayerName(BroadPhaseLayer _inLayer) const
{
	switch ((BroadPhaseLayer::Type)_inLayer)
	{
	case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
	case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
	default:													JPH_ASSERT(false); return "INVALID";
	}
}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
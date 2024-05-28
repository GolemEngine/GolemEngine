#pragma once

#include <Jolt/Jolt.h>
#include "Physic/layers.h"

using namespace JPH;

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLInterface final : public BroadPhaseLayerInterface
{
private:
	BroadPhaseLayer m_ObjectToBroadPhase[ObjectLayers::NUM_LAYERS];

public:
	BPLInterface();

	virtual uint GetNumBroadPhaseLayers() const override;

	virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer _inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer _inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

};

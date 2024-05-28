#pragma once

#include "dll.h"


enum ViewMode
{
    DEFAULT,
    WIREFRAME
};

class GOLEM_ENGINE_API ViewportTools
{
public:
    static inline ViewMode currentViewMode = DEFAULT;

private:
    // Private constructors and destructor to make static class
    ViewportTools() = delete;
    ViewportTools(ViewportTools& _other) = delete;
    ViewportTools(ViewportTools&& _other) = delete;
    ViewportTools& operator=(ViewportTools& _other) = delete;
    ViewportTools& operator=(ViewportTools&& _other) = delete;
    void operator=(const ViewportTools&) = delete;
    ~ViewportTools() = delete;
};

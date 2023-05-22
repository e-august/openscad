#pragma once

#include <memory>

#include "OffscreenContext.h"

namespace offscreen_old {

std::shared_ptr<OffscreenContext> CreateOffscreenContextGLX(
    uint32_t width, uint32_t height, uint32_t majorGLVersion, 
    uint32_t minorGLVersion, bool gles, bool compatibilityProfile);

}  // namespace offscreen_old

#pragma once

#include <iostream>
#include "../imguiInclude.h"


class FrameBuffer
{
public:

    FrameBuffer() : mFBO{ 0 }, mDepthId{ 0 } {};

    FrameBuffer(int32_t width, int32_t height) : mFBO{ 0 }, mDepthId{ 0 }
    {create_buffers(width, height); }

    ~FrameBuffer() {
        
    }

    void create_buffers(int32_t width, int32_t height);

    void delete_buffers();

    void bind();

    void unbind();

    uint32_t get_texture();

protected:
    uint32_t mFBO = 0;
    uint32_t mTexId = 0;
    uint32_t mDepthId = 0;
    int32_t mWidth = 0;
    int32_t mHeight = 0;
};
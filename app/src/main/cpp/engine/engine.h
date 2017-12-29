#pragma once

#include <android/asset_manager_jni.h>

#include <vector>
#include "matrix.h"
#include "../pch.h"

#include "updater.h"
#include "mesh.h"

namespace Engine {

    class Engine {
    protected:

        static UINT sThreadCounter;

        UINT m_id;

        GLint m_viewport[4];

    public:

        Engine() {
            m_id = sThreadCounter;
            ++sThreadCounter;
        }

        virtual ~Engine() {
            clear();
        }

        virtual void clear() {};

        virtual bool init(AAssetManager *pAssetManager) = 0;

        virtual bool render() = 0;

        virtual void resume() = 0;

        virtual void pause() = 0;

        virtual void updateSize(int width, int height)
        {
            LOGI("Engine(size updated: %d, %d )", width, height);
            m_viewport[0] = m_viewport[1] = 0;
            m_viewport[2] = width;
            m_viewport[3] = height;
        }

        virtual void touch(float /*x*/, float /*y*/) {}
        virtual void setOffset(float /*x*/, float /*y*/) {}
    };
}

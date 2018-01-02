#pragma once

#include "../engine/engine.h"
#include "updater.h"

namespace Electric {

    class ElectricEngine : public Engine::Engine
    {
    private:

        typedef Math::Matrix Matrix;
        typedef Math::Vector3 Vector3;

        std::mutex  m_matrixMutex;
        Matrix      m_view;
        Matrix      m_projection;

        bool        m_vpDirty;
        Matrix      m_vp;

        Updater     m_updater;

    public:

        ElectricEngine()
        {
            updateView(0.5f);
            updateProjection(1, 1);
        }

        virtual void clear() override;
        virtual bool init(AAssetManager* pAssetManager) override;
        virtual bool render() override;

        virtual void resume() override { m_updater.resume(); }

        virtual void pause() override { m_updater.pause(); }

        virtual void updateSize(int width, int height) override
        {
            Engine::updateSize(width, height);
            updateProjection(static_cast<float>(width), static_cast<float>(height));
        }

        virtual void setOffset(float x, float y) override
        {
            Engine::setOffset(x, y);
            updateView(x);
        }

        virtual void updateView(float offset)
        {
            Matrix rotation;
            Matrix::setRotate(rotation, 0, 360.0f * (offset - 0.5f), 0);
            Vector3 pos = Matrix::transform(rotation, {0,0,-100.0f});
            {
                std::lock_guard<std::mutex> _(m_matrixMutex);
                Matrix::lookAt(m_view, pos, {0, 0, 1}, {0, 1, 0});
                m_vpDirty = true;
            }
        }

        virtual void updateProjection(float w, float h)
        {
            {
                std::lock_guard<std::mutex> _(m_matrixMutex);
                Matrix::perspective(m_projection, 90.0f, w / h, 10, 110);
                m_vpDirty = true;
            }
        }
    };

}
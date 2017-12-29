#pragma once

#include "mesh.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

namespace Engine {

    class Updater {
    private:

        static UINT sThreadCounter;

        UINT m_id;

        std::atomic_bool m_running;
        std::thread m_workThread;

        void start();

        void stop();

        void run();

        virtual void advance(float dt) = 0;

    public:

        Updater()
                : m_running(false) {
            m_id = sThreadCounter;
            ++sThreadCounter;
            LOGI("Engine( Created: %d )", m_id);
        }

        virtual ~Updater() {
            stop();
        }

        virtual bool init();

        void resume() {
            start();
        }

        void pause() {
            stop();
        }
    };
}
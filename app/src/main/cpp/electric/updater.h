#pragma once

#include "../engine/updater.h"

namespace Electric {

    class Updater : public Engine::Updater
    {
    private:

    protected:

        virtual void advance(float dt) override;

    public:


    };
}

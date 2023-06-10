#pragma once
#include <mathfu/vector.h>

namespace CrossCraft {

    class Entity {
        public:
        Entity();
        virtual ~Entity();

        virtual auto update(double dt) -> void = 0;
        virtual auto draw() -> void = 0;

        mathfu::Vector<float, 3> position;
        mathfu::Vector<float, 2> rotation;
        uint16_t eid;
        void* data;
    };

}
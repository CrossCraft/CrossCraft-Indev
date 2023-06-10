#include <Entity/Entity.hpp>

namespace CrossCraft {
        Entity::Entity() {
            position = mathfu::Vector<float, 3>(0, 0, 0);
            rotation = mathfu::Vector<float, 2>(0, 0);
            data = nullptr;
        }

        Entity::~Entity() {
        }
}
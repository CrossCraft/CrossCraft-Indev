#pragma once
#include <map>
#include <Entity/Entity.hpp>

namespace CrossCraft {

    class EntityManager {
        public:
        EntityManager();
        ~EntityManager();

        auto update(double dt) -> void;
        auto draw() -> void;

        auto add(Entity* entity) -> void;
        auto remove(Entity* entity) -> void;

        static auto get() -> EntityManager& {
            static EntityManager instance;
            return instance;
        }

        static auto get_entities() -> std::map<int, Entity*>& {
            return get().entities;
        }

        private:
        std::map<int, Entity*> entities;
    };

}
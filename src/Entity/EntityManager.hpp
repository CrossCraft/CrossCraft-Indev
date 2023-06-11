#pragma once

#include <map>
#include <Entity/Entity.hpp>
#include <Player/Player.hpp>

namespace CrossCraft {

    class EntityManager {
    public:
        EntityManager();

        ~EntityManager();

        auto update(Player *player, double dt) -> void;

        auto draw() -> void;

        auto add(Entity *entity) -> void;

        auto remove(Entity *entity) -> void;

        auto remove(uint16_t eid) -> void;

        static auto get() -> EntityManager & {
            static EntityManager instance;
            return instance;
        }

        [[maybe_unused]] static auto get_entities() -> std::map<int, Entity *> & {
            return get().entities;
        }

        auto handle_teleport(uint16_t eid, float x, float y, float z, float vx, float vy, float vz, uint8_t yaw,
                             uint8_t pitch) -> void;

    private:
        std::map<int, Entity *> entities;
    };

}
#include <Entity/EntityManager.hpp>
#include <Utilities/Logger.hpp>

namespace CrossCraft {

EntityManager::EntityManager() {
    }

    EntityManager::~EntityManager() {
    }

    auto EntityManager::update(double dt) -> void {
        for(auto& [id, entity] : entities) {
            entity->update(dt);
        }
    }

    auto EntityManager::draw() -> void {
        for(auto& [id, entity] : entities) {
            entity->draw();
        }
    }

    auto EntityManager::add(Entity* entity) -> void {
        entities.emplace(entity->eid, entity);
        SC_APP_INFO("Added entity {0}", entity->eid);
    }

    auto EntityManager::remove(Entity* entity) -> void {
        entities.erase(entity->eid);
    }
}
#include <Entity/EntityManager.hpp>
#include <Utilities/Logger.hpp>
#include <Entity/ItemEntity.hpp>
#include <Player/Inventory.hpp>

namespace CrossCraft {

    EntityManager::EntityManager() {

    }

    EntityManager::~EntityManager() {

    }

    auto EntityManager::update(Player* player, double dt) -> void {
        for(auto& [id, entity] : entities) {
            entity->update(dt);

            auto diff = player->position - entity->position;
            auto length = diff.Length();

            if(length < 1) {
                ItemEntity* item;
                if((item = dynamic_cast<ItemEntity*>(entity)) && item->data != nullptr && item->data->count > 0) {
                    if(Inventory::get().try_add_item(*item->data)) {
                        CC_Event_Push_DestroyEntity(item->eid);
                    }
                }
            }
        }
    }

    auto EntityManager::draw() -> void {
        for(auto& [id, entity] : entities) {
            entity->draw();
        }
    }

    auto EntityManager::add(Entity* entity) -> void {
        entities.emplace(entity->eid, entity);
    }

    auto EntityManager::remove(Entity* entity) -> void {
        entities.erase(entity->eid);
    }
    auto EntityManager::remove(uint16_t eid) -> void {
        entities.erase(eid);
    }
}
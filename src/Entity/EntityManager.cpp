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
                if((item = dynamic_cast<ItemEntity*>(entity)) && item->data != nullptr && item->data->count > 0 && item->lifetimer > 0.5f) {
                    if(Inventory::get().try_add_item(*item->data)) {
                        CC_Event_Push_DestroyEntity(item->eid);
                    }
                }
            }
        }
    }

    auto EntityManager::handle_teleport(uint16_t eid, float x, float y, float z, float vx, float vy, float vz, uint8_t yaw, uint8_t pitch) -> void {
        if(entities.find(eid) != entities.end()) {
            auto diff = entities[eid]->position - mathfu::Vector<float, 3>(x, y, z);
            if(diff.Length() > 0.2f) {
                entities[eid]->position = mathfu::Vector<float, 3>(x, y, z);
            }
            entities[eid]->velocity = mathfu::Vector<float, 3>(vx, vy, vz);
            entities[eid]->rotation = mathfu::Vector<float, 3>((float)yaw / 255.0f * 360.0f, (float)pitch / 255.0f * 360.0f, 0.0f);
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
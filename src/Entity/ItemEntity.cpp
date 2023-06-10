#include <Entity/ItemEntity.hpp>
#include <ModelRenderer.hpp>

namespace CrossCraft {

    ItemEntity::ItemEntity() : Entity() {
        data = nullptr;
        lifetimer = 0;
    }
    ItemEntity::~ItemEntity() {

    }

    auto ItemEntity::update(double dt) -> void {
        lifetimer += dt;
        rotation.y += dt * 30.0f;
    }

    auto ItemEntity::draw() -> void {
        if(data != nullptr) {
            if(data->count > 0) {
                if(data->id < 256) {
                    position.y += sinf(lifetimer) * 0.1f;
                    ModelRenderer::get().draw_block(data->id, position, mathfu::Vector<float, 3>(rotation.x, rotation.y, 0));
                    position.y -= sinf(lifetimer) * 0.1f;
                } else {
                    position.y += sinf(lifetimer) * 0.1f;
                    ModelRenderer::get().draw_item(data->id, position, mathfu::Vector<float, 3>(rotation.x, rotation.y, 0));
                    position.y -= sinf(lifetimer) * 0.1;
                }
            }
        }
    }

}
#include <Player/Inventory.hpp>
#include <ModelRenderer.hpp>
#include <Player/Player.hpp>

namespace CrossCraft {

    Inventory::Inventory() {
        hotbar = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{ {0.0f, 0.0f} , {182.0f, 22.0f}},
                Rendering::Rectangle{ {0.0f, 0.0f}, {182.0f/ 256.0f, 22.0f / 256.0f}}
        );
        hotbar->set_layer(0);
        hotbar_select = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{ {0.0f, 0.0f} , {24.0f, 24.0f}},
                Rendering::Rectangle{ {0.0f, 22.0f / 256.0f}, {24.0f/ 256.0f, 24.0f / 256.0f}}
        );
        hotbar_select->set_layer(1);

        font_render = create_refptr<FontRender>();

        for (auto& i : item_array) {
            i.id = 0;
            i.count = 0;
        }
    }

    Inventory::~Inventory() {

    }

    auto Inventory::draw(double dt) -> void {

    }

    auto Inventory::increment_selection(std::any p) -> void {
        auto new_idx = get().selection_idx + 1;
        if (new_idx > 8) {
            new_idx = 0;
        }
        get().selection_idx = new_idx;
    }
    auto Inventory::decrement_selection(std::any p) -> void {
        auto new_idx = get().selection_idx - 1;
        if (new_idx < 0) {
            new_idx = 8;
        }
        get().selection_idx = new_idx;
    }

    auto Inventory::set_selection(std::any p) -> void {
        auto new_idx = std::any_cast<SelData>(p).idx;
        if (new_idx < 0 || new_idx > 8) {
            return;
        }
        get().selection_idx = new_idx;
    }

    auto Inventory::try_add_item(ItemData item) -> bool {
        //TODO: Check if item is stackable
        //TODO: Rewrite so hotbar is prioritized
        //TODO: Rewrite so items are added from left to right
        for (int it = 44; it >= 9; it--) {
            auto& i = item_array[it];
            if (i.id == item.id && i.count < 64) {
                i.count += item.count;
                return true;
            }
        }
        for (int it = 44; it >= 9; it--) {
            auto& i = item_array[it];
            if (i.id == 0) {
                i = item;
                return true;
            }
        }

        SC_APP_DEBUG("Inventory full");
        return false;
    }

    auto Inventory::draw_block_hand(mathfu::Vector<float, 3> position, mathfu::Vector<float, 2> rotation, double dt) -> void {
        if(item_array[selection_idx + 36].id != 0 && item_array[selection_idx + 36].count != 0) {
            auto id = item_array[selection_idx + 36].id;

            if(id < 256) {
                ModelRenderer::get().draw_block_hand(id, position, rotation.xy());
            } else {
                ModelRenderer::get().draw_item_hand(id, position, rotation.xy());
            }
        }
    }

    auto Inventory::drop_selection(std::any p) -> void {
        auto& itm = get().item_array[get().selection_idx + 36];

        if(itm.id == 0 || itm.count == 0) {
            return;
        }

        auto player = std::any_cast<Player*>(p);

        auto rx = mathfu::Matrix<float, 3>::RotationX(player->rotation.x / 180.0f * M_PI);
        auto ry = mathfu::Matrix<float, 3>::RotationY((-player->rotation.y + 180.0f) / 180.0f * M_PI);

        mathfu::Vector<float, 3> view = mathfu::Vector<float, 3>(0, 0, 1);
        view = rx * view;
        view = ry * view;

        auto pos = player->position + view;
        auto vel = view * 5.0f;

        auto itmCopy = itm;
        itmCopy.count = 1;

        CC_Event_Push_SpawnItem(itmCopy, pos.x - 0.5f, pos.y + 1.0f, pos.z - 0.5f, vel.x, vel.y, vel.z);

        itm.count--;
        if(itm.count == 0) {
            itm.id = 0;
        }
    }

    auto Inventory::draw_hotbar(double dt) -> void {
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({ 149.0f, 1.0f, 0.0f });
        hotbar->draw();

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({ 148.0f + 20.0f * (float)selection_idx, 0.0f, 0.0f });
        hotbar_select->draw();

        font_render->clear();
        for(int i = 0; i < 9; i++) {
            auto slot = item_array[i + 36];

            if (slot.id == 0 || slot.count == 0) {
                continue;
            }


            Rendering::RenderContext::get().matrix_clear();
            if(slot.id < 256) {
                ModelRenderer::get().draw_block_isometric(slot.id, {160.0f + 20.0f * (float)i, 12.0f, 2.0f});
            } else {
                ModelRenderer::get().draw_item_isometric(slot.id, {160.0f + 20.0f * (float)i, 12.0f, 2.0f});
            }

            if(slot.count > 1) {
                font_render->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(slot.count),
                                               {168.0f + 20.0f * (float) i, 4.0f}, 10.0f);
            }
        }

        Rendering::RenderContext::get().matrix_clear();
        font_render->build();
        font_render->draw();
    }

}
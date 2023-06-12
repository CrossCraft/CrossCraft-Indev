#include <Player/Inventory.hpp>
#include <ModelRenderer.hpp>
#include <Player/Player.hpp>
#include <GLFW/glfw3.h>
#include <Utilities/Input.hpp>
#include <Player/InGameMenu.hpp>

namespace GI {
    extern GLFWwindow *window;
}


namespace CrossCraft {
    using namespace Stardust_Celeste;
    using namespace Stardust_Celeste::Utilities;

    Inventory::Inventory() {
        for(int i = 9; i < 45; i++) {
            item_array[i].id = i;
            item_array[i].count = i;
        }

        open = false;
        hotbar = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{{0.0f,   0.0f},
                                     {182.0f, 22.0f}},
                Rendering::Rectangle{{0.0f,            0.0f},
                                     {182.0f / 256.0f, 22.0f / 256.0f}}
        );
        hotbar->set_layer(0);
        hotbar_select = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{{0.0f,  0.0f},
                                     {24.0f, 24.0f}},
                Rendering::Rectangle{{0.0f,           22.0f / 256.0f},
                                     {24.0f / 256.0f, 24.0f / 256.0f}}
        );
        hotbar_select->set_layer(1);

        background_rectangle = create_refptr<Rendering::Primitive::Rectangle>(
                Rendering::Rectangle{{0,   0},
                                     {480, 272}},
                Rendering::Color{{70, 70, 96, 216}}, 10);

        inventory_background = create_scopeptr<Graphics::G2D::Sprite>(ResourcePack::get().get_texture("inventory_overlay"),
                                                                    Rendering::Rectangle{{152, 53}, {176, 166}},
                                                                    Rendering::Rectangle{{0, 0}, {176.0f / 256.0f, 166.0f / 256.0f}});

        inventory_background->set_layer(10);

        font_render_hotbar = create_refptr<FontRender>();
        font_render_inventory = create_refptr<FontRender>();

        pickup_slot.id = 0;
        pickup_slot.count = 0;
    }

    Inventory::~Inventory() = default;

    auto Inventory::draw(double dt) -> void {
        if(!open) return;

        background_rectangle->draw();
        inventory_background->draw();

        font_render_inventory->clear();
        for (int i = 9; i < 45; i++) {
            auto slot = item_array[i];

            if (slot.id == 0 || slot.count == 0) {
                continue;
            }

            int extra_off = 0;
            if(i >= 36) {
                extra_off = -4;
            }

            Rendering::RenderContext::get().matrix_clear();
            if (slot.id < 256) {
                ModelRenderer::get().draw_block_isometric(slot.id, {168 + 18.0f * (float)(i % 9), 127.0f - 18.0f * (float)(i/9 - 1) + (float)extra_off, 12.0f});
            } else {
                ModelRenderer::get().draw_item_isometric(slot.id, {168 + 18.0f * (float)(i % 9), 127.0f - 18.0f * (float)(i/9 - 1) + (float)extra_off, 12.0f});
            }

            if (slot.count > 1) {
                font_render_inventory->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(slot.count),
                                                        {177.0f + 18.0f * (float)(i % 9), 118.0f - 18.0f * (float)(i/9 - 1) + (float)extra_off}, 20.0f);
            }
        }

        // Draw stuff in lower slots
        if(item_array[0].id != 0) {
            Rendering::RenderContext::get().matrix_clear();
            if (item_array[0].id < 256) {
                ModelRenderer::get().draw_block_isometric(item_array[0].id, {143 + 153 + 8, 53 + (176 - 36 - 18),  12.0f});
            } else {
                ModelRenderer::get().draw_item_isometric(item_array[0].id, {143 + 153, 53 + (176 - 36 - 18), 12.0f});
            }

            if (item_array[0].count > 1) {
                font_render_inventory->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(item_array[0].count),
                                                         {143 + 153 + 17, 53 + (176 - 36 - 18 - 9)}, 20.0f);
            }
        }

        // Draw the crafting slots
        for(int i = 0; i < 4; i++) {
            if(item_array[i + 1].id != 0) {
                Rendering::RenderContext::get().matrix_clear();
                if (item_array[i + 1].id < 256) {
                    ModelRenderer::get().draw_block_isometric(item_array[i + 1].id, {static_cast<float>(248 + 18 * (i%2)), static_cast<float>(185 - 18 * (i/2)),  12.0f});
                } else {
                    ModelRenderer::get().draw_item_isometric(item_array[i + 1].id, {static_cast<float>(248 + 18 * (i%2)), static_cast<float>(185 - 18 * (i/2)), 12.0f});
                }

                if (item_array[i + 1].count > 1) {
                    font_render_inventory->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(item_array[i + 1].count),
                                                             {static_cast<float>(248 + 18 * (i%2) + 9), static_cast<float>(185 - 18 * (i/2)) - 9}, 20.0f);
                }
            }
        }

        // Draw the armor slots
        for(int i = 0; i < 4; i++) {
            if(item_array[i + 5].id != 0) {
                Rendering::RenderContext::get().matrix_clear();
                if (item_array[i + 5].id < 256) {
                    ModelRenderer::get().draw_block_isometric(item_array[i + 5].id, {168, static_cast<float>(203 - 18 * i),  12.0f});
                } else {
                    ModelRenderer::get().draw_item_isometric(item_array[i + 5].id, {168, static_cast<float>(203 - 18 * i), 12.0f});
                }

                if (item_array[i + 5].count > 1) {
                    font_render_inventory->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(item_array[i + 5].count),
                                                             {168 + 9, static_cast<float>(203 - 18 * i) - 9}, 20.0f);
                }
            }
        }

        // Draw the selected block(s)
        if(pickup_slot.id != 0) {
            if (pickup_slot.id < 256) {
                ModelRenderer::get().draw_block_isometric(pickup_slot.id, {mouse_pos, 24.0f});
            } else {
                ModelRenderer::get().draw_item_isometric(pickup_slot.id, {mouse_pos, 24.0f});
            }
            if(pickup_slot.count > 1) {
                font_render_inventory->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT,
                                                         std::to_string(pickup_slot.count),
                                                         mouse_pos - mathfu::Vector<float, 2>(-8.0f, 8.0f), 32.0f);
            }
        }

        Rendering::RenderContext::get().matrix_clear();
        font_render_inventory->build();
        font_render_inventory->draw();
    }

    auto Inventory::increment_selection(const std::any p) -> void {
        auto new_idx = get().selection_idx + 1;
        if (new_idx > 8) {
            new_idx = 0;
        }
        get().selection_idx = new_idx;
    }

    auto Inventory::decrement_selection(const std::any p) -> void {
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

    auto Inventory::update() -> void {
        if(get().is_mouse_pressed) {
            get().is_mouse_pressed = false;
        } else if(get().is_mouse_held && !get().is_mouse_pressed) {
            get().is_mouse_held = false;
        }

        auto cX = Input::get_axis("Mouse", "X");
        auto cY = Input::get_axis("Mouse", "Y");
        mouse_pos = {cX * 480.0f, 272.0f + -cY * 272.0f};
    }

    auto get_inventory_index(mathfu::Vector<int, 2> pd) -> int {
        int idx = -1;
        pd.x -= 159;
        pd.y -= 53;

        if(pd.y > 83) {
            //This is the main inventory area.
            int ix = pd.x / 18.0f;
            int iy = pd.y / 18.0f - 5.0f + 0.277f;

            idx = ix + iy * 9 + 9;
        } else {
            if(pd.x < 18) {
                // This is the armor area
                int ix = pd.x / 18.0f;
                int iy = pd.y / 20.0f;

                idx = ix + iy + 5;
            } else if(pd.x > 62 && pd.x < 62 + 48) {
                pd.x -= 62 + 18;
                pd.y -= 24;

                // This is the crafting area
                int ix = pd.x / 18.0f;
                int iy = pd.y / 20.0f;

                idx = ix + iy * 2 + 1;
            } else if(pd.x > 140 && pd.x < 160 && pd.y > 34 && pd.y < 54) {
                idx = 0;
            }
        }

        return idx;
    }

    auto Inventory::left_action(std::any p) -> void {
        auto pd = std::any_cast<PassOnData>(p);
        pd.cX *= 480;
        pd.cY *= 272;
        get().mouse_pos = {pd.cX, pd.cY};

        get().is_mouse_pressed = true;

        if(!get().is_mouse_held) {
            get().is_mouse_held = true;
        } else {
            get().is_mouse_held = true;
            return;
        }

        // Out of bound
        if(pd.cX < 159 || pd.cX > (159 + 161))
            return;

        // Out of bound
        if(pd.cY < 53 || pd.cY > (53 + 157))
            return;

        auto idx = get_inventory_index({static_cast<int>(pd.cX), static_cast<int>(pd.cY)});

        if(idx == -1) return;

        SC_APP_INFO("INVENTORY SELECTED: {}", idx);

        auto& inst = get();
        if(inst.pickup_slot.id == 0 && inst.item_array[idx].id != 0) {
            inst.pickup_slot = inst.item_array[idx];
            inst.item_array[idx] = {0, 0, 0};
        } else if(inst.pickup_slot.id != 0 && inst.item_array[idx].id == 0) {
            inst.item_array[idx] = inst.pickup_slot;
            inst.pickup_slot = {0, 0, 0};
        }

        /*
        auto ix = (pd.x - 168.0f) / 18.0f;

        int extra_off = 0;
        if(i >= 36) {
            extra_off = -4;
        }

        Rendering::RenderContext::get().matrix_clear();
        if (slot.id < 256) {
            ModelRenderer::get().draw_block_isometric(slot.id, {168 + 18.0f * (float)(i % 9), 127.0f - 18.0f * (float)(i/9 - 1) + (float)extra_off, 12.0f});
        } else {
            ModelRenderer::get().draw_item_isometric(slot.id, {168 + 18.0f * (float)(i % 9), 127.0f - 18.0f * (float)(i/9 - 1) + (float)extra_off, 12.0f});
        }


        SC_APP_INFO("Left action: {} {}",pd.x, pd.y);
         */
    }

    auto Inventory::right_action(std::any p) -> void {
        auto pd = std::any_cast<PassOnData>(p);
        pd.cX *= 480;
        pd.cY *= 272;

        SC_APP_INFO("Right action: {} {}",pd.cX, pd.cY);
    }

    auto Inventory::try_add_item(ItemData item) -> bool {
        //TODO: Check if item is stackable
        //TODO: Rewrite so hotbar is prioritized
        //TODO: Rewrite so items are added from left to right
        for (int it = 44; it >= 9; it--) {
            auto &i = item_array[it];
            if (i.id == item.id && i.count < 64) {
                i.count += item.count;
                return true;
            }
        }
        for (int it = 44; it >= 9; it--) {
            auto &i = item_array[it];
            if (i.id == 0) {
                i = item;
                return true;
            }
        }

        SC_APP_DEBUG("Inventory full");
        return false;
    }

    auto Inventory::draw_block_hand(const mathfu::Vector<float, 3>& position, const mathfu::Vector<float, 2>& rotation,
                                    double dt) -> void {
        if (item_array[selection_idx + 36].id != 0 && item_array[selection_idx + 36].count != 0) {
            auto id = item_array[selection_idx + 36].id;

            if (id < 256) {
                ModelRenderer::get().draw_block_hand(id, mathfu::Vector<float, 2>());
            } else {
                ModelRenderer::get().draw_item_hand(id, mathfu::Vector<float, 2>());
            }
        }
    }

    auto Inventory::drop_selection(std::any p) -> void {
        auto &itm = get().item_array[get().selection_idx + 36];

        if (itm.id == 0 || itm.count == 0) {
            return;
        }

        auto player = std::any_cast<Player *>(p);

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
        if (itm.count == 0) {
            itm.id = 0;
        }
    }

    auto Inventory::draw_hotbar(double dt) -> void {
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({149.0f, 1.0f, 0.0f});
        hotbar->draw();

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({148.0f + 20.0f * (float) selection_idx, 0.0f, 0.0f});
        hotbar_select->draw();

        font_render_hotbar->clear();
        for (int i = 0; i < 9; i++) {
            auto slot = item_array[i + 36];

            if (slot.id == 0 || slot.count == 0) {
                continue;
            }


            Rendering::RenderContext::get().matrix_clear();
            if (slot.id < 256) {
                ModelRenderer::get().draw_block_isometric(slot.id, {160.0f + 20.0f * (float) i, 12.0f, 2.0f});
            } else {
                ModelRenderer::get().draw_item_isometric(slot.id, {160.0f + 20.0f * (float) i, 12.0f, 2.0f});
            }

            if (slot.count > 1) {
                font_render_hotbar->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_RIGHT, std::to_string(slot.count),
                                                      {169.0f + 20.0f * (float) i, 3.0f}, 10.0f);
            }
        }

        Rendering::RenderContext::get().matrix_clear();
        font_render_hotbar->build();
        font_render_hotbar->draw();
    }

    auto Inventory::toggle_inventory(const std::any p) -> void {
        if(InGameMenu::get().is_open())
            return;

        get().open = !get().open;

        if (get().open) {
            glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Input::set_differential_mode("Mouse", false);
        } else {
            Input::set_cursor_center();
            glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Input::set_differential_mode("Mouse", true);
        }
    }
}
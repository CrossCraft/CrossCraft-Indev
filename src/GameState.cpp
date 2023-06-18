#include <GameState.hpp>
#include <CC/core.h>
#include <Utilities/Controllers/KeyboardController.hpp>
#include <Utilities/Controllers/MouseController.hpp>
#include <ResourcePack.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>
#include <ModelRenderer.hpp>
#include <Entity/EntityManager.hpp>
#include <CC/eventloop.h>
#include <CC/alphaindev.pb-c.h>
#include <CC/eventpackets.h>
#include <Chunk/ChunkMeta.hpp>

namespace CrossCraft {

    bool gLoggedIn = false;

    using namespace Stardust_Celeste::Utilities;

    auto GameState::on_action_left_up(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        Player::break_block_up(player);
    }

    auto GameState::on_action_left(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (InGameMenu::get().is_open() || Inventory::is_open()) {
            InGameMenu::left_click_command(p);
        } else {
            Player::break_block(player);
        }
    }

    auto GameState::on_action_right(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (InGameMenu::get().is_open() || Inventory::is_open()) {
            InGameMenu::right_click_command();
        } else {
            Player::place_block(player);
        }
    }

    void GameState::on_start() {
        ResourcePack::get().add_pack("resourcepacks/default.zip");
        ResourcePack::get().load();
        ModelRenderer::init();


        // Initialize the core library.
        CC_Core_Init();

        ChunkMeta::get().generate();

        // Create the server-side event loop for SP
        auto event_loop = CC_EventLoop_Init();

        // Create the client-side event loop for SP
        client_event_loop = CC_EventLoop_Init();

        // Create the server-side data bus for SP
        auto server_bus = SharedDataBus_Init();

        // Create che client-side data bus for SP
        auto data_bus = SharedDataBus_Init();

        // Set the inbound bus for the event loop.
        CC_EventLoop_SetInboundBus(event_loop, &data_bus->bus, data_bus);
        CC_EventLoop_SetOutboundBus(event_loop, &server_bus->bus, server_bus);

        // Set the inbound bus for the client event loop.
        CC_EventLoop_SetInboundBus(client_event_loop, &server_bus->bus, server_bus);
        CC_EventLoop_SetOutboundBus(client_event_loop, &data_bus->bus, data_bus);

        // Set the server event loop for the core library.
        CC_Core_SetEventLoop(event_loop);

        // Send the handshake to the server.
        EventPacket handshake_packet = CC_EventPacket_Create_Handshake("IridescentRose", false);
        CC_EventLoop_PushPacketOutbound(client_event_loop, &handshake_packet);
        CC_EventLoop_Update(client_event_loop);

        CC_EventLoop_RegisterHandler(client_event_loop, CC_PACKET_TYPE_HANDSHAKE, [](void* loop, EventPacket* packet) {
            auto handshake = packet->data.handshake_sc.response.data;
            if(handshake[0] == '-') {
                SC_APP_INFO("Handshake successful! Logging in!");
                auto p = CC_EventPacket_Create_LoginClient("IridescentRose", "password");
                CC_EventLoop_PushPacketOutbound((EventLoop*)loop, &p);
            } else {
                throw std::runtime_error("Handshake failed!");
            }
        });

        CC_EventLoop_RegisterHandler(client_event_loop, CC_PACKET_TYPE_LOGIN, [](void* loop, EventPacket* packet) {
            gLoggedIn = true;
        });
        CC_EventLoop_RegisterHandler(client_event_loop, CC_PACKET_TYPE_SPAWN_POSITION, [](void* loop, EventPacket* packet) {
            auto spawn = packet->data.spawn_position;
            SC_APP_INFO("Spawn position: {0}, {1}, {2}", spawn.x, spawn.y, spawn.z);
        });
        CC_EventLoop_RegisterHandler(client_event_loop, CC_PACKET_TYPE_PLAYER_POSITION_AND_LOOK, [](void* loop, EventPacket* packet) {
            auto p = packet->data.player_position_and_look_sc;
            SC_APP_INFO("Player position: {0}, {1}, {2}", p.x, p.y, p.z);
            SC_APP_INFO("Player look: {0}, {1}", p.yaw, p.pitch);
        });
        CC_EventLoop_RegisterHandler(client_event_loop, CC_PACKET_TYPE_UPDATE_HEALTH, [](void* loop, EventPacket* packet) {
            auto p = packet->data.update_health;
            SC_APP_INFO("Player health: {0}", p.health);
        });

        while(!gLoggedIn) {
            CC_Core_Update(1);
            CC_EventLoop_Update(client_event_loop);
        }

        // Send initial player position.
        CC_Event_Push_PlayerUpdate(PLAYER_SELF, 128.0f, 48.0f, 128.0f, 0.0f, 0.0f, false);

        // Make sure player position is handled.
        CC_Core_Update(0.0);

        // Create the player.
        player = create_refptr<Player>();
        InGameMenu::init();

        // Setup controls
        kb_controller = new Utilities::Input::KeyboardController();

        //TODO: Clean this up
        kb_controller->add_command({(int) Input::Keys::W, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::S, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::A, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::D, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::W, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_forward, player.get()});
        kb_controller->add_command({(int) Input::Keys::S, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_backward, player.get()});
        kb_controller->add_command({(int) Input::Keys::A, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_left, player.get()});
        kb_controller->add_command({(int) Input::Keys::D, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_right, player.get()});
        kb_controller->add_command({(int) Input::Keys::Space, KeyFlag::Press | KeyFlag::Held},
                                   {Player::jump, player.get()});
        kb_controller->add_command({(int) Input::Keys::Enter, KeyFlag::Press}, {World::save, nullptr});
        kb_controller->add_command({(int) Input::Keys::Escape, KeyFlag::Press}, {InGameMenu::toggle_command, nullptr});
        kb_controller->add_command({(int) Input::Keys::Num1, KeyFlag::Press}, {Inventory::set_selection, SelData{0}});
        kb_controller->add_command({(int) Input::Keys::Num2, KeyFlag::Press}, {Inventory::set_selection, SelData{1}});
        kb_controller->add_command({(int) Input::Keys::Num3, KeyFlag::Press}, {Inventory::set_selection, SelData{2}});
        kb_controller->add_command({(int) Input::Keys::Num4, KeyFlag::Press}, {Inventory::set_selection, SelData{3}});
        kb_controller->add_command({(int) Input::Keys::Num5, KeyFlag::Press}, {Inventory::set_selection, SelData{4}});
        kb_controller->add_command({(int) Input::Keys::Num6, KeyFlag::Press}, {Inventory::set_selection, SelData{5}});
        kb_controller->add_command({(int) Input::Keys::Num7, KeyFlag::Press}, {Inventory::set_selection, SelData{6}});
        kb_controller->add_command({(int) Input::Keys::Num8, KeyFlag::Press}, {Inventory::set_selection, SelData{7}});
        kb_controller->add_command({(int) Input::Keys::Num9, KeyFlag::Press}, {Inventory::set_selection, SelData{8}});
        kb_controller->add_command({(int) Input::Keys::Q, KeyFlag::Press}, {Inventory::drop_selection, player.get()});
        kb_controller->add_command({(int) Input::Keys::E, KeyFlag::Press}, {Inventory::toggle_inventory, player.get()});

        // TODO: Clean This Up
        mb_controller = new Utilities::Input::MouseController();
        mb_controller->add_command({(int) Input::MouseButtons::Left, KeyFlag::Release | KeyFlag::Untouched},
                                   {on_action_left_up, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::Left, KeyFlag::Press | KeyFlag::Held},
                                   {on_action_left, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::Right, KeyFlag::Press}, {on_action_right, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::MWheelUp, KeyFlag::Press},
                                   {Inventory::decrement_selection, nullptr});
        mb_controller->add_command({(int) Input::MouseButtons::MWheelDown, KeyFlag::Press},
                                   {Inventory::increment_selection, nullptr});

        Input::add_controller(kb_controller);
        Input::add_controller(mb_controller);
        Input::set_differential_mode("Mouse", true);
        Input::set_cursor_center();

        Rendering::RenderContext::get().set_mode_3D();

        world = create_refptr<World>();
    }


    void GameState::check_poll_input(double dt) {
        poll_time += dt;
        if(poll_time > (1.0f / 240.0f)) {
            Input::update();
            poll_time = 0.0f;
            Inventory::get().update();
        }
    }

    void GameState::tick(double dt) {
        tick_time += dt;

        if(tick_time > (1.0f / 20.0f)) {
            tick_time = 0.0f;
            player->tick();
        }

        CC_EventLoop_Update(client_event_loop);
    }

    void GameState::on_update(Core::Application *app, double dt) {
        check_poll_input(dt);
        tick(dt);

        player->update(dt);
        world->update(dt);
        EntityManager::get().update(player.get(), dt);

        CC_Core_Update(dt);

        handle_events(dt);
    }


    void GameState::handle_events(double dt) {
        CC_Event *event;
        while ((event = CC_Event_Poll()) != nullptr) {
            switch (event->type) {
                case CC_EVENT_SET_BLOCK: {
                    world->handle_block_update(event->data.set_block.x, event->data.set_block.y,
                                               event->data.set_block.z);
                    break;
                }

                case CC_EVENT_SPAWN_ITEM: {
                    world->handle_spawn_item(event->data.spawn_item.eid, event->data.spawn_item.x,
                                             event->data.spawn_item.y, event->data.spawn_item.z,
                                             event->data.spawn_item.vx, event->data.spawn_item.vy,
                                             event->data.spawn_item.vz, event->data.spawn_item.item);
                    break;
                }

                case CC_EVENT_DESTROY_ENTITY: {
                    EntityManager::get().remove(event->data.destroy_entity.eid);
                    break;
                }

                case CC_EVENT_SET_PLAYER_HEALTH: {
                    Player::handle_health_update(HealthUpdate{player.get(), event->data.set_player_health.health});
                    break;
                }

                case CC_EVENT_ENTITY_TELEPORT: {
                    EntityManager::get().handle_teleport(event->data.entity_teleport.eid, event->data.entity_teleport.x,
                                                         event->data.entity_teleport.y, event->data.entity_teleport.z,
                                                         event->data.entity_teleport.vx, event->data.entity_teleport.vy,
                                                         event->data.entity_teleport.vz,
                                                         event->data.entity_teleport.yaw,
                                                         event->data.entity_teleport.pitch);
                    break;
                }

                default:
                    SC_APP_INFO("Unhandled event type: {}", event->type);
                    break;
            }
        }
    }

    void GameState::setup_2d_rendering() {
        Rendering::RenderContext::get().set_mode_2D();
        Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -200, 200);
        GI::clearDepth();
        GI::disable(GI_CULL_FACE);
    }

    void GameState::setup_3d_rendering() {
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().set_mode_3D();
    }

    float x = 0;

    void GameState::on_draw(Core::Application *app, double dt) {
        world->draw();

        Inventory::get().draw_block_hand(player->position, player->rotation, dt);

        setup_2d_rendering();

        player->draw(dt);
        Inventory::get().draw(dt);

        setup_3d_rendering();
    }

    void GameState::on_cleanup() {
        CC_Core_Term();
        CC_EventLoop_Destroy(client_event_loop);
    }

} // namespace CrossCraft
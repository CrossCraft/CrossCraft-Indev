#include <ResourcePack.hpp>
#include <Rendering/RenderContext.hpp>
#include <Rendering/Texture.hpp>
#include <algorithm>
#include <utility>

using namespace Stardust_Celeste;

namespace CrossCraft {

    ResourcePack::ResourcePack() = default;

    ResourcePack::~ResourcePack() = default;

    void ResourcePack::add_pack(const std::string& pack) {
        unzFile pack_file = unzOpen(pack.c_str());
        if (pack_file == nullptr) {
            return;
        }

        packs[pack] = pack_file;
        packQueue.push_back(pack);
    }

    [[maybe_unused]] void ResourcePack::remove_pack(const std::string& pack) {
        unzClose(packs[pack]);
        packs.erase(pack);

        auto it = std::find(packQueue.begin(), packQueue.end(), pack);
        if (it != packQueue.end()) {
            packQueue.erase(it);
        }
    }

    auto ResourcePack::get_resource(const std::string& resource) -> ResourceValue * {
        if (resources.find(resource) == resources.end()) {
            return nullptr;
        }

        return &resources[resource];
    }

    auto ResourcePack::get_texture(std::string resource) -> uint32_t {
        auto res = get_resource(std::move(resource));
        if (res == nullptr) {
            return -1;
        }

        if (res->type == ResourceType::TEXTURE) {
            return res->data.texture_value;
        }

        return -1;
    }

    auto ResourcePack::get_path_index(std::string path) -> int {
        int path_index = -1;

        for (long int idx = packQueue.size() - 1; idx >= 0; idx--) {
            if (unzLocateFile(packs[packQueue[idx]], path.c_str(), 0) == UNZ_OK) {
                path_index = idx;
                break;
            }
        }

        if (path_index == -1) {
            throw std::runtime_error("Resource not found: " + path);
        }

        return path_index;
    }


    auto ResourcePack::read_file_to_buffer(std::string packname, size_t& length) -> uint8_t* {
        unzOpenCurrentFile(packs[packname]);

        unz_file_info file_info;
        unzGetCurrentFileInfo(packs[packname], &file_info, nullptr, 0, nullptr, 0, nullptr, 0);

        length = file_info.uncompressed_size;
        auto dataBuffer = malloc(sizeof(uint8_t) * length);

        unzReadCurrentFile(packs[packname], dataBuffer, length);
        return (uint8_t *)dataBuffer;
    }

    auto ResourcePack::load_resource(std::string name, ResourceType type, const std::string& path, bool needPix) -> void {
        if (type == ResourceType::TEXTURE) {
            auto packname = packQueue[get_path_index(path)];

            size_t arrayLength;
            auto dataBuffer = read_file_to_buffer(packname, arrayLength);
            auto result = Rendering::TextureManager::get().load_texture_ram(dataBuffer, arrayLength,
                                                                            SC_TEX_FILTER_NEAREST,
                                                                            SC_TEX_FILTER_NEAREST, true, false, false, needPix);

            free(dataBuffer);

            ResourceValue res{};
            res.type = ResourceType::TEXTURE;
            res.data.texture_value = result;

            resources.emplace(name, res);
        }
    }

    void ResourcePack::load() {
        load_resource("default", ResourceType::TEXTURE, "assets/minecraft/textures/default.png", true);
        load_resource("terrain", ResourceType::TEXTURE, "assets/minecraft/textures/terrain.png");
        load_resource("water_overlay", ResourceType::TEXTURE, "assets/minecraft/textures/water.png");
        load_resource("gui_common", ResourceType::TEXTURE, "assets/minecraft/textures/gui/gui.png");
        load_resource("items", ResourceType::TEXTURE, "assets/minecraft/textures/gui/items.png");
        load_resource("icons", ResourceType::TEXTURE, "assets/minecraft/textures/gui/icons.png");
        load_resource("death_overlay", ResourceType::TEXTURE, "assets/crosscraft/textures/overlay.png");
        load_resource("inventory_overlay", ResourceType::TEXTURE, "assets/minecraft/textures/gui/inventory.png");
    }

    void ResourcePack::cleanup() {
        for (auto &[key, res]: resources) {
            if (res.type == ResourceType::TEXTURE) {
                Rendering::TextureManager::get().delete_texture(res.data.texture_value);
            }
            //TODO: Load Sounds
        }

        resources.clear();
    }

    [[maybe_unused]] void ResourcePack::reload() {
        cleanup();
        load();
    }
}
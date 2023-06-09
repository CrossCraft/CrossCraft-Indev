#include <ResourcePack.hpp>
#include <Rendering/RenderContext.hpp>
#include <Rendering/Texture.hpp>
#include <algorithm>

using namespace Stardust_Celeste;

namespace CrossCraft{

    ResourcePack::ResourcePack() {

    }

    ResourcePack::~ResourcePack() {

    }

    void ResourcePack::add_pack(std::string pack) {
        unzFile pack_file = unzOpen(pack.c_str());
        if (pack_file == nullptr) {
            return;
        }

        packs[pack] = pack_file;
        packQueue.push_back(pack);
    }

    void ResourcePack::remove_pack(std::string pack) {
        unzClose(packs[pack]);
        packs.erase(pack);

        auto it = std::find(packQueue.begin(), packQueue.end(), pack);
        if(it != packQueue.end()) {
            packQueue.erase(it);
        }
    }

    auto ResourcePack::get_resource(std::string resource) -> ResourceValue* {
        if(resources.find(resource) == resources.end()) {
            return nullptr;
        }

        return &resources[resource];
    }

    auto ResourcePack::get_texture(std::string resource) -> uint32_t {
        auto res = get_resource(resource);
        if(res == nullptr) {
            return -1;
        }

        if(res->type == ResourceType::TEXTURE) {
            return res->data.texture_value;
        }

        return -1;
    }

    auto ResourcePack::load_resource(std::string name, ResourceType type, std::string path) -> void {
        if(type == ResourceType::TEXTURE) {
            int path_index = -1;

            for(long int idx = packQueue.size() - 1; idx >= 0; idx--) {
                if(unzLocateFile(packs[packQueue[idx]], path.c_str(), 0) == UNZ_OK) {
                    path_index = idx;
                    break;
                }
            }

            if(path_index == -1) {
                throw std::runtime_error("Resource not found: " + path);
            }

            auto packname = packQueue[path_index];

            unzOpenCurrentFile(packs[packname]);

            unz_file_info file_info;
            unzGetCurrentFileInfo(packs[packname], &file_info, nullptr, 0, nullptr, 0, nullptr, 0);

            auto arrayLength = file_info.uncompressed_size;
            auto dataBuffer = new uint8_t[arrayLength];

            unzReadCurrentFile(packs[packname], dataBuffer, arrayLength);

            auto result = Rendering::TextureManager::get().load_texture_ram(dataBuffer, arrayLength, SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true);
            delete[] dataBuffer;

            ResourceValue res;
            res.type = ResourceType::TEXTURE;
            res.data.texture_value = result;

            resources.emplace(name, res);
        }
    }

    void ResourcePack::load() {
        load_resource("default", ResourceType::TEXTURE, "assets/minecraft/textures/default.png");
        load_resource("terrain", ResourceType::TEXTURE, "assets/minecraft/textures/terrain.png");
        load_resource("water_overlay", ResourceType::TEXTURE, "assets/minecraft/textures/water.png");
        load_resource("gui_common", ResourceType::TEXTURE, "assets/minecraft/textures/gui/gui.png");
    }

    void ResourcePack::cleanup() {
        for(auto& [key, res] : resources) {
            if(res.type == ResourceType::TEXTURE) {
                Rendering::TextureManager::get().delete_texture(res.data.texture_value);
            }
            //TODO: Load Sounds
        }

        resources.clear();
    }

    void ResourcePack::reload() {
        cleanup();
        load();
    }
}
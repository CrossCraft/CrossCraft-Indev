#pragma once
#include <string>
#include <queue>
#include <Utilities/Singleton.hpp>
#include <zlib.h>
#include <minizip/unzip.h>
#include <map>
#include <cstdint>

namespace CrossCraft{

    enum class ResourceType {
        TEXTURE,
        SOUND_EFFECT,
        MUSIC
    };

    struct ResourceValue {
        ResourceType type;
        union {
            uint32_t texture_value;
            void* audio_data;
        } data;
    };

    class ResourcePack {
    public:
        virtual ~ResourcePack();

        static ResourcePack& get() {
            static ResourcePack rspck;
            return rspck;
        }

        void load();
        void cleanup();
        void reload();

        void add_pack(std::string pack);
        void remove_pack(std::string pack);

        auto get_resource_type(std::string resource) -> ResourceValue*;
        auto get_texture(std::string resource) -> uint32_t;

    private:
        ResourcePack();

        auto load_resource(std::string name, ResourceType type, std::string path) -> ResourceValue*;

        std::map<std::string, ResourceValue> resources;
        std::vector<std::string> packQueue;
        std::map<std::string, unzFile> packs;
    };

}
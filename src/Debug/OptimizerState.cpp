#include <Debug/OptimizerState.hpp>
#include <ResourcePack.hpp>
#include <ModelRenderer.hpp>
#include <Chunk/ChunkMeta.hpp>

namespace CrossCraft{
    using namespace Stardust_Celeste;

    void OptimizerState::on_start() {
        ResourcePack::get().add_pack("resourcepacks/default.zip");
        ResourcePack::get().load();
        ModelRenderer::init();

        // Initialize the core library.
        CC_Core_Init();

        std::unordered_map<uint64_t, ChunkStack *> chunks;

        size_t maxX, maxY, maxZ;
        CC_World_GetChunkSize(&maxX, &maxY, &maxZ);
        SC_APP_INFO("Max chunk size: {0}, {1}, {2}", maxX, maxY, maxZ);

        for (uint32_t x = 0; x < maxX; x++) {
            for (uint32_t z = 0; z < maxZ; z++) {
                mathfu::Vector<int, 2> pos(x, z);

                uint64_t id = ((uint64_t) pos.x) << 32 | (uint64_t) pos.y;

                if (chunks.find(id) == chunks.end()) {
                    chunks.emplace(id, new ChunkStack(x, z));
                } else {
                    chunks.emplace(id, chunks[id]);
                    chunks.erase(id);
                }
            }
        }

        const int SAMPLE_COUNT = 1;
        SC_APP_INFO("BEGINNING GENERATION");
        auto startTime = std::chrono::high_resolution_clock::now();

        ChunkMeta::get().generate();
        auto endTimeMeta = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < SAMPLE_COUNT; i++) {
            for (auto &[id, chunk]: chunks) {
                chunk->generate();
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
        auto finalTime = duration / 1000000000.0;
        auto durationMeta = std::chrono::duration_cast<std::chrono::nanoseconds>(endTimeMeta - startTime).count();
        auto finalTimeMeta = durationMeta / 1000000000.0;
        auto finalChunkTime = (duration - durationMeta) / 1000000000.0;
        auto avgTime = finalChunkTime / (double)(SAMPLE_COUNT * chunks.size());
        auto cps = std::trunc(1.0 / avgTime);

        SC_APP_INFO("ENDING GENERATION");
        std::cout << "Number Samples: " << SAMPLE_COUNT << std::endl;
        std::cout << "Number Chunks: " << chunks.size() << std::endl;
        std::cout << "Total Chunks: " << chunks.size() * SAMPLE_COUNT << std::endl;
        std::cout << "Total Time: " << finalTime << " seconds" << std::endl;
        std::cout << "Meta Time: " << finalTimeMeta << " seconds" << std::endl;
        std::cout << "Chunk Time: " << finalChunkTime << " seconds" << std::endl;
        std::cout << "Average Time: " << avgTime << " seconds" << std::endl;
        std::cout << "Average Chunks Per Second: " << cps << std::endl;

    }

    void OptimizerState::on_update(Core::Application *app, double dt) {
        app->exit();
    }

    void OptimizerState::on_draw(Core::Application *app, double dt) {

    }

    void OptimizerState::on_cleanup() {

    }
}
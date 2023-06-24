#include <World/Sky.hpp>

namespace CrossCraft {

    Sky::Sky() {
        mesh = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();
        mesh2 = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();
        generate(Rendering::Color{{255, 255, 255, 255}}, Rendering::Color{{255, 255, 255, 255}});
    }


    void Sky::generate(Rendering::Color color, Rendering::Color color2) {
        mesh->delete_data();
        mesh->vertices.clear();
        mesh->indices.clear();
        uint16_t idx_counter = 0;

        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {

                const std::array<float, 12> cFace{0, 0, 0, 1, 0, 0,
                                                  1, 0, 1, 0, 0, 1};

                const std::array<float, 8> uvs{0, 0, 1, 0, 1, 1, 0, 1};

                mesh->vertices.push_back(Rendering::Vertex{
                        uvs[0] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[1] * (1 / 128.f) + ((float)z / 128.f),
                        color,
                        cFace[0] * 16 + x * 16,
                        0,
                        cFace[2] * 16 + z * 16,
                });

                mesh->vertices.push_back(Rendering::Vertex{
                        uvs[2] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[3] * (1 / 128.f) + ((float)z / 128.f),
                        color,
                        cFace[3] * 16 + x * 16,
                        0,
                        cFace[5] * 16 + z * 16,
                });

                mesh->vertices.push_back(Rendering::Vertex{
                        uvs[4] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[5] * (1 / 128.f) + ((float)z / 128.f),
                        color,
                        cFace[6] * 16 + x * 16,
                        0,
                        cFace[8] * 16 + z * 16,
                });

                mesh->vertices.push_back(Rendering::Vertex{
                        uvs[6] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[7] * (1 / 128.f) + ((float)z / 128.f),
                        color,
                        cFace[9] * 16 + x * 16,
                        0,
                        cFace[11] * 16 + z * 16,
                });

                mesh->indices.push_back(idx_counter);
                mesh->indices.push_back(idx_counter + 1);
                mesh->indices.push_back(idx_counter + 2);
                mesh->indices.push_back(idx_counter + 2);
                mesh->indices.push_back(idx_counter + 3);
                mesh->indices.push_back(idx_counter + 0);
                idx_counter += 4;
            }
        }

        mesh->setup_buffer();


        mesh2->delete_data();
        mesh2->vertices.clear();
        mesh2->indices.clear();
        idx_counter = 0;

        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {

                const std::array<float, 12> cFace{0, 0, 0, 1, 0, 0,
                                                  1, 0, 1, 0, 0, 1};

                const std::array<float, 8> uvs{0, 0, 1, 0, 1, 1, 0, 1};

                mesh2->vertices.push_back(Rendering::Vertex{
                        uvs[0] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[1] * (1 / 128.f) + ((float)z / 128.f),
                        color2,
                        cFace[0] * 16 + x * 16,
                        0,
                        cFace[2] * 16 + z * 16,
                });

                mesh2->vertices.push_back(Rendering::Vertex{
                        uvs[2] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[3] * (1 / 128.f) + ((float)z / 128.f),
                        color2,
                        cFace[3] * 16 + x * 16,
                        0,
                        cFace[5] * 16 + z * 16,
                });

                mesh2->vertices.push_back(Rendering::Vertex{
                        uvs[4] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[5] * (1 / 128.f) + ((float)z / 128.f),
                        color2,
                        cFace[6] * 16 + x * 16,
                        0,
                        cFace[8] * 16 + z * 16,
                });

                mesh2->vertices.push_back(Rendering::Vertex{
                        uvs[6] * (1 / 128.f) + ((float)x / 128.f),
                        uvs[7] * (1 / 128.f) + ((float)z / 128.f),
                        color2,
                        cFace[9] * 16 + x * 16,
                        0,
                        cFace[11] * 16 + z * 16,
                });

                mesh2->indices.push_back(idx_counter);
                mesh2->indices.push_back(idx_counter + 1);
                mesh2->indices.push_back(idx_counter + 2);
                mesh2->indices.push_back(idx_counter + 2);
                mesh2->indices.push_back(idx_counter + 3);
                mesh2->indices.push_back(idx_counter + 0);
                idx_counter += 4;
            }
        }

        mesh2->setup_buffer();
    }

    Rendering::Color HSL2RGB(float h, float sl, float l) {
        double v;
        double r, g, b;
        r = l;   // default to gray
        g = l;
        b = l;
        v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
        if (v > 0)
        {
            double m;
            double sv;
            int sextant;
            double fract, vsf, mid1, mid2;

            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.0;
            sextant = (int)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;
            switch (sextant)
            {
                case 0:
                    r = v;
                    g = mid1;
                    b = m;
                    break;
                case 1:
                    r = mid2;
                    g = v;
                    b = m;
                    break;
                case 2:
                    r = m;
                    g = v;
                    b = mid1;
                    break;
                case 3:
                    r = m;
                    g = mid2;
                    b = v;
                    break;
                case 4:
                    r = mid1;
                    g = m;
                    b = v;
                    break;
                case 5:
                    r = v;
                    g = m;
                    b = mid2;
                    break;
            }
        }
        return Rendering::Color{{static_cast<u8>(r * 255.0f), static_cast<u8>(g * 255.0f), static_cast<u8>(b * 255.0f), 255}};
    }

    auto blend(float source, float destination) {
        const float blendFactor = 0.29f;
        return destination + (source - destination) * blendFactor;
    }

    auto blendColor(Rendering::Color source, Rendering::Color destination) {
        return Rendering::Color{{
            static_cast<u8>(blend((float) source.rgba.r / 255.0f, (float) destination.rgba.r / 255.0f) * 255.0f),
            static_cast<u8>(blend((float) source.rgba.g / 255.0f, (float) destination.rgba.g / 255.0f) * 255.0f),
            static_cast<u8>(blend((float) source.rgba.b / 255.0f, (float) destination.rgba.b / 255.0f) * 255.0f),
            static_cast<u8>(blend((float) source.rgba.a / 255.0f, (float) destination.rgba.a / 255.0f) * 255.0f)
        }};
    }

    void Sky::draw(mathfu::Vector<float, 3> cam_pos, uint64_t tick) {
        const float temp = 0.8f / 3;

        // X should be based on tick time
        float x = static_cast<float>(tick % 24000) / 24000.0f - 0.25f;
        if (x < 0) x = 0;
        if (x > 1) x = 1;
        auto CelestialAngle = x + ((1.0 - ((float)cos(x * 3.14159) + 1.0) / 2.0) - x) / 3.0;

        auto mod = (float)cos(CelestialAngle * 2.0 * 3.14159) * 2.0 + 0.5f;
        if (mod < 0) mod = 0;
        if (mod > 1) mod = 1;
        auto BrightnessModifier = mod;

        auto BaseColor = HSL2RGB(0.6222222f - temp * 0.05f, 0.65f + temp * 0.1f, BrightnessModifier * 0.707f);
        auto WorldSkyColor = BaseColor;

        float translateY = (float)cos(CelestialAngle * 2 * 3.14159) * 2 + 0.5f;
        auto r = (0.7529412f * translateY * 0.94f + 0.06f) * 255.0f;
        auto g = (0.8470588f * translateY * 0.94f + 0.06f) * 255.0f;
        auto b = (1.0f * translateY * 0.91f + 0.09f) * 255.0f;

        if(r < 0) r = 0;
        if(r > 255) r = 255;
        if(g < 0) g = 0;
        if(g > 255) g = 255;
        if(b < 0) b = 0;
        if(b > 255) b = 255;

        auto WorldFogColor = Rendering::Color{{static_cast<u8>(r), static_cast<u8>(g), static_cast<u8>(b), 255}};

        auto AtmosphereColor = blendColor(WorldSkyColor, WorldFogColor);

        if(tick != last_tick) {
            last_tick = tick;

            auto colVec = mathfu::Vector<float, 3>(WorldSkyColor.rgba.r / 255.0f, WorldSkyColor.rgba.g / 255.0f, WorldSkyColor.rgba.b / 255.0f);
            colVec *= mathfu::Vector<float, 3>(0.2f, 0.2f, 0.6f);
            colVec += mathfu::Vector<float, 3>(0.04f, 0.04f, 0.1f);
            auto VoidColor = Rendering::Color{{static_cast<u8>(colVec.x * 255.0f), static_cast<u8>(colVec.y * 255.0f), static_cast<u8>(colVec.z * 255.0f), 255}};
            generate(WorldSkyColor, VoidColor);
        }

        Rendering::RenderContext::get().set_color(AtmosphereColor);

        GI::fog_color(AtmosphereColor);
        GI::enable(GI_FOG);
        GI::disable(GI_TEXTURE_2D);

        auto realY = 80.0f - cam_pos.y;

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(mathfu::Vector<float, 3>(-256.0f, realY, -256.0f) + cam_pos);
        Rendering::RenderContext::get().matrix_scale({2.0f, 1.0f, 2.0f});

        mesh->draw();

        auto realY2 = -16.0f - cam_pos.y;

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(mathfu::Vector<float, 3>(-256.0f, realY2, -256.0f) + cam_pos);
        Rendering::RenderContext::get().matrix_scale({2.0f, 1.0f, 2.0f});
        GI::set_culling_mode(true, false);
        mesh2->draw();
        GI::set_culling_mode(true, true);
    }
}
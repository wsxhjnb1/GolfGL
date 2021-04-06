#pragma once

#include <precomp.h>

class Light
{
public:
    Light(const Light &) = delete;
    Light(Light &&) = delete;
    void operator=(const Light &) = delete;

    static void Init()
    {
        ms_Light = std::move(std::unique_ptr<Light>());
    }

    inline static Light &Get()
    {
        return *ms_Light;
    }

    inline static glm::vec3 LightPosition{1.2f, 1.2f, 2.0f};
    inline static glm::vec3 Ambient{0.6f, 0.6f, 0.6f};
    inline static glm::vec3 Diffuse{0.5f, 0.5f, 0.5f};
    inline static glm::vec3 Specular{0.3f, 0.3f, 0.3f};

private:
    inline static std::unique_ptr<Light> ms_Light = nullptr;

    Light() = default;
};

#define LIGHT Light::Get()

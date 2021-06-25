#pragma once

#include "lightData.h"

/** @class Light
 * Singleton object with static access
 *
 * /par Options:
 * - LIGHT: static ref to object
 * - LIGHTS: static ref to object
 *
 */
class Light
{
public:
    Light(const Light &) = delete;
    Light(Light &&) = delete;
    void operator=(const Light&) = delete;

    static void Init();
    
    inline static Light& Get()
    {
        return *ms_Light;
    }

    PhongLight& operator[](const int idx) const;
        
private:
    inline static std::unique_ptr<Light> ms_Light = nullptr;
    inline static std::vector<PhongLight> ms_PLights;

    Light() = default;
};

#define LIGHT Light::Get()

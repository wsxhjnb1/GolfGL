#include <precomp.h>
#include "Light.h"

void Light::Init()
{
    ms_Light = std::move(std::unique_ptr<Light>());
    ms_PLights = std::move( std::vector<PhongLight>(allLights) );
}

PhongLight& Light::operator[](const int idx) const
{
    return ms_PLights[idx];
}

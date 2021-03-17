#include <precomp.h>

#include "Light.h"

void Light::Init()
{    
    ms_Light = std::move( std::unique_ptr<Light>() );
}
#include <precomp.h>
#include "Light.h"
#include "lightData.h"

std::vector<PhongLight> Light::ms_PLights { allLights };
PhongLight Light::ms_Flashlight{ FLASHLIGHT };

void Light::Init()
{
    ms_Light = std::move(std::unique_ptr<Light>());    
}

PhongLight& Light::operator[](const int idx) const
{
    if(idx == ms_PLights.size())
        return ms_Flashlight;
    return ms_PLights[idx];
}

int Light::Size() const
{
    return ms_PLights.size();
}

inline std::string opIdx(std::string name, int i) { return name.append("[" + std::to_string(i) + "]"); }

inline void setOneLight(Render::Shader& shader, const PhongLight& light, int idx)
{
    shader.setVec3(opIdx("lightPos", idx),               light.Position);
    shader.setVec3(opIdx("light",    idx) + ".ambient",  light.Ambient);
    shader.setVec3(opIdx("light",    idx) + ".diffuse",  light.Diffuse);
    shader.setVec3(opIdx("light",    idx) + ".specular", light.Specular);            
    shader.setVec3(opIdx("light",    idx) + ".color",    light.Color);                        
    shader.SetValue(opIdx("light",   idx) + ".type", static_cast<unsigned>(light.Type));
    if(light.Type == LightType::POINT || light.Type == LightType::SPOT)
    {
        shader.SetValue(opIdx("light",   idx) + ".linear", light.linear);
        shader.SetValue(opIdx("light",   idx) + ".quadratic", light.quadratic);
        if(light.Type == LightType::SPOT)
        {        
            shader.setVec3(opIdx("lightDir", idx), light.Direction);
            shader.SetValue(opIdx("light",   idx) + ".cutoff", glm::cos(glm::radians(light.CutoffAngle) ));
            shader.SetValue(opIdx("light",   idx) + ".outerCutoff",
                                glm::cos(glm::radians(light.CutoffAngle * 1.4f) ));
        }
    }
}

void Light::SetAllLights(Render::Shader &shader)
{
    int numLights = LIGHT.Size();    
    int i = 0;
    for(int l = 0; l < numLights; l++)    
        if(LIGHT[l].Type != LightType::DISABLED)
            setOneLight(shader, LIGHT[l], i++);
    if(Flashlight)
    {               
        ms_Flashlight.Position = CAMERA.GetCameraPos();
        ms_Flashlight.Direction = CAMERA.GetCameraFront();
        setOneLight(shader, ms_Flashlight, i++);
    }    

    shader.SetValue("numLights", i);
}

PhongLight & Light::GetFlashlight()
{
    return ms_Flashlight;
}





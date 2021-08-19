#pragma once

namespace Render {
    class Shader;
}

enum class LightType : unsigned
{
    DIRECTIONAL = 0u,
    POINT,
    SPOT,
    DISABLED,
};

/** @struct PhongLight
 * Universal struct for light data
 */
struct PhongLight
{
    glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Color;
    LightType Type = LightType::DISABLED;

    glm::vec3 Direction{0.f};
    float CutoffAngle = 30.f;
   
    float linear = 0.022f;        
    float quadratic = 0.0019f;
};

/** @class Light
 * Singleton object with static access
 *
 * /par Options:
 * - LIGHT: static ref to object 
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
    /** Index access operator
     *
     *  @param idx index idx in range [0, size]
     *
     *  @return Light object from lights collection if idx is in [0, size-1]
     *  @return Flashlight if idx size
     */
    PhongLight& operator[](const int idx) const;
    [[nodiscard]] int Size() const;
    
    static void SetAllLights(Render::Shader& shader);
    inline static bool Flashlight = false;
    static PhongLight& GetFlashlight();
    
private:
    inline static std::unique_ptr<Light> ms_Light = nullptr;
    static std::vector<PhongLight> ms_PLights;    
    static PhongLight ms_Flashlight;    

    Light() = default;

#ifdef _DEBUG
    friend class Window::DebugWindow;
#endif
};

#define LIGHT Light::Get()

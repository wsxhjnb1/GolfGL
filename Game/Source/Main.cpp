#include <precomp.h>

int main()
{
    Log::Init();
    Light::Init();
    Camera::Init();
    Render::Renderer renderer;
    Input input;

    if (!renderer.Init() || !input.Init(renderer.GetWindow()))
    {
        return 1;
    }

    while (renderer.IsRunning())
    {
        input.ProcessInput();
        renderer.Update();
    }

    return 0;
}

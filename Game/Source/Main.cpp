int main()
{
    Log::Init();
    Render::Renderer renderer;
    
    if (!renderer.Init())
        return 1;

    while (renderer.IsRunning())
    {
        renderer.Update();
    }

    return 0;
}
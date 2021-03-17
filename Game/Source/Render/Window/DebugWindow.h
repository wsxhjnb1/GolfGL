#pragma once

namespace Window {

	/* Wrapper for ImGui window */
	class DebugWindow
	{
	public:
		
		bool Init();
		bool Update();

		void Destroy();

		DebugWindow()  = default;
		~DebugWindow() = default;

	private:
		void DrawLightMenu();
		void DrawGFXSettings();

		bool m_DebugMenuActive = true;		
		ImVec4 clear_color{ 0.45f, 0.55f, 0.60f, 1.00f };
		
	};

} // Window
#pragma once

namespace Render {
	class fbData
	{
		friend class Framebuffer;
		inline static float vertices[24] = {

			-1.0f,  1.0f,	0.0f, 1.0f,
			-1.0f, -1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f,	1.0f, 0.0f,

			-1.0f,  1.0f,	0.0f, 1.0f,
			 1.0f, -1.0f,	1.0f, 0.0f,
			 1.0f,  1.0f,	1.0f, 1.0f
		};
	};
}

Render::fbData data;
#pragma once
#include <Windows.h>
#include "Config.h"
#include "Painter.h"

namespace LAB1 {
	/// 
	/// The function of this class hierarcy is to draw different kinds of graphics object 
	/// 
	class Sprite {
	protected:
		Painter* m_Painter;
		RECT m_spriteRect;
		Sprite(Painter* painter, RECT spriteRect) : m_Painter{ painter }, m_spriteRect{spriteRect} {}
	public:
		virtual void Draw() = 0;
		RECT GetSpriteRect() const { return m_spriteRect; }
		virtual void SetSpriteRect(RECT newRect);
	};

}
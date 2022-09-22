#pragma once
#include "../Sprite.h"

namespace LAB1 {
	class RectangleSprite final: public Sprite {
	private:
		//RECT m_rect;
		COLOR m_brushColor;
	public:
		RectangleSprite(Painter* painter, RECT rect, COLOR rectColor);
		void Draw() override;
		//virtual void SetSpriteRect(RECT newRect) override;
	};
}
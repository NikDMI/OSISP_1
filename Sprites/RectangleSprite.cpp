#include "RectangleSprite.h"


namespace LAB1 {
	RectangleSprite::RectangleSprite(Painter* painter, RECT rect, COLOR rectColor) :Sprite(painter, rect),
		m_brushColor{rectColor} {}

	void RectangleSprite::Draw() {
		m_Painter->SetBrushColor(m_brushColor);
		m_Painter->Rectangle(m_spriteRect);
	}
}
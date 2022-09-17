#include "Sprite.h"

namespace LAB1 {

	EllipseSprite::EllipseSprite(Painter* painter) : 
		EllipseSprite(painter, { 0, 0 }, DEFAULT_RADIUS, DEFAULT_RADIUS) {}

	EllipseSprite::EllipseSprite(Painter* painter, POINT origin, int hRadius, int vRadius) :
		Sprite{painter},
		m_CentralPoint{ origin },m_HorizonralR{ hRadius }, m_VerticalR{ vRadius }  {}

	void EllipseSprite::Draw(RECT destRect) {

	}
}
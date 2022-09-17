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
		Sprite(Painter* painter) : m_Painter{painter} {}
	public:
		virtual void Draw(RECT destRect) = 0;
	};

	class EllipseSprite : public Sprite {
		POINT m_CentralPoint;
		int m_HorizonralR;
		int m_VerticalR;
		const int DEFAULT_RADIUS = 30;
	public:
		void Draw(RECT destRect) override;
		EllipseSprite(Painter* painter);
		EllipseSprite(Painter* painter, POINT origin, int hRadius, int vRadius);
	};

}
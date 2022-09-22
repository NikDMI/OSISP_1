#pragma once
#include "../Moveable.h"

namespace LAB1 {

	class StaticMoveBehaviour final: public MoveBehavior {
	private:
		//sizes of offset throught one unit move
		INT m_horizontalDistance = 0; 
		INT m_verticalDistance = 0;
	public:
		StaticMoveBehaviour(RECT clientRect, RECT objectRect);
		void SetOffsetValues(INT dx, INT dy) noexcept;
		void RefreshRectCoords() override;
	};

}
#include "StaticMoveBehaviour.h"

namespace LAB1 {

	StaticMoveBehaviour::StaticMoveBehaviour(RECT clientRect, RECT objectRect): MoveBehavior{ clientRect,objectRect } {}

	void StaticMoveBehaviour::SetOffsetValues(INT dx, INT dy) noexcept {
		this->m_horizontalDistance = dx;
		this->m_verticalDistance = dy;
	}

	void StaticMoveBehaviour::RefreshRectCoords() {
		if (m_horizontalDistance || m_verticalDistance) {
			OffsetRect(&m_ObjectRect, m_horizontalDistance, m_verticalDistance);
			this->SetValidObjectRect();
			m_horizontalDistance = 0; m_verticalDistance = 0;
		}
	}

}
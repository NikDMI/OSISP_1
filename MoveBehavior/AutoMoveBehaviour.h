#pragma once
#include "../Moveable.h"


namespace LAB1 {

	class AutoMoveBehavior : public MoveBehavior {
	private:
		double m_maxVerticalSpeed = 700.0f;//speed per second
		double m_maxHorizontalSpeed = 700.0f;
		double m_verticalSpeed = m_maxVerticalSpeed;//speed per second
		double m_horizontalSpeed = m_maxHorizontalSpeed;
		Timer timer{};

		void GenerateNewSpeedValues(ClipResult clipping);
	public:
		AutoMoveBehavior(RECT clientRect, RECT objectRect);
		void RefreshRectCoords() override;
		void StartMoving();
		void StopMoving();
	};

}
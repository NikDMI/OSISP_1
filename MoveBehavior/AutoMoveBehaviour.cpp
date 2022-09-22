#include "AutoMoveBehaviour.h"

namespace LAB1 {

	AutoMoveBehavior::AutoMoveBehavior(RECT clientRect, RECT objectRect) : MoveBehavior{ clientRect,objectRect } {
		timer.Reset();
		timer.StopTimer();
	}

	void AutoMoveBehavior::StartMoving() {
		timer.RestartTimer();
	}

	void AutoMoveBehavior::StopMoving() {
		timer.StopTimer();
	}

	void AutoMoveBehavior::GenerateNewSpeedValues(ClipResult clipping) {
		double verticalMultiplyer = (double)(rand() % 51) / 100.0f + 0.5;
		double horizontalMultiplyer = (double)(rand() % 51) / 100.0f + 0.5;
		if (clipping.isClipedLeftRightSide)
		{
			m_horizontalSpeed = (m_horizontalSpeed > 0) ? -m_maxHorizontalSpeed * horizontalMultiplyer : m_maxHorizontalSpeed * horizontalMultiplyer;
			m_verticalSpeed = (m_verticalSpeed > 0) ? m_maxVerticalSpeed * verticalMultiplyer : -m_maxVerticalSpeed * verticalMultiplyer;

		}
		if (clipping.isClipedTopBtmSide) {
			m_verticalSpeed = (m_verticalSpeed > 0) ? -m_maxVerticalSpeed * verticalMultiplyer : m_maxVerticalSpeed * verticalMultiplyer;
			m_horizontalSpeed = (m_horizontalSpeed > 0) ? m_maxHorizontalSpeed * horizontalMultiplyer : -m_maxHorizontalSpeed * horizontalMultiplyer;
		}
	}

	void AutoMoveBehavior::RefreshRectCoords() {
		double dt = timer.Tick();
		OffsetRect(&m_ObjectRect, dt * m_horizontalSpeed, dt * m_verticalSpeed);
		ClipResult clipping = this->SetValidObjectRect();
		this->GenerateNewSpeedValues(clipping);
	}
}
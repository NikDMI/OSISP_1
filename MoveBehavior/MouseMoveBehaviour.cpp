#include "MouseMoveBehaviour.h"

namespace LAB1 {
	///////////////////////   MouseMoveBehavior   ////////////////////////////

	MouseMoveBehavior::MouseMoveBehavior(RECT clientRect, RECT objectRect) :MoveBehavior{ clientRect,objectRect } {}

	void MouseMoveBehavior::CaptureMouseCoord() {
		GetCursorPos(&m_captureMousePosition);
		if (!isCapture) {
			isCapture = true;
			m_startCaptureObjectRect = m_ObjectRect;
		}
	}

	void MouseMoveBehavior::UncaptureMouseCoord() {
		if (isCapture) {
			isCapture = false;
		}
	}

	void MouseMoveBehavior::RefreshRectCoords() {
		if (isCapture) {
			POINT mousePos; GetCursorPos(&mousePos);
			UINT dx = mousePos.x - m_captureMousePosition.x;
			UINT dy = mousePos.y - m_captureMousePosition.y;
			m_ObjectRect = m_startCaptureObjectRect;
			OffsetRect(&m_ObjectRect, dx, dy);
			this->SetValidObjectRect();
		}
	}
}
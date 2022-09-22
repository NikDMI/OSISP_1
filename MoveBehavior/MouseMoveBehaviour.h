#pragma once
#include "../Moveable.h"

namespace LAB1 {
	class MouseMoveBehavior final : public MoveBehavior {
	private:
		bool isCapture = false;
		POINT m_captureMousePosition{};
		RECT m_startCaptureObjectRect{};
	public:
		MouseMoveBehavior(RECT clientRect, RECT objectRect);
		void CaptureMouseCoord();
		void UncaptureMouseCoord();
		void RefreshRectCoords() override;
	};
}
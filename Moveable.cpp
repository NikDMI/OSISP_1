#include "Moveable.h"

namespace LAB1 {
	MoveBehavior::MoveBehavior(RECT clientRect, RECT objectRect) :m_ClientRect{ clientRect }, m_ObjectRect{objectRect} {
		if (!isSuitableSize()) throw ExceptionBadSize{};
		m_ObjectRectHeight = objectRect.bottom - objectRect.top;
		m_ObjectRectWidth = objectRect.right - objectRect.left;
		if (m_ObjectRectHeight <= 0 || m_ObjectRectWidth <= 0) throw ExceptionBadArgs{};
	}

	MoveBehavior::~MoveBehavior() {
	}

	bool MoveBehavior::isSuitableSize() {
		return m_ObjectRect.left>=m_ClientRect.left && m_ObjectRect.top>=m_ClientRect.top &&
			m_ObjectRect.right<=m_ClientRect.right && m_ObjectRect.bottom<=m_ClientRect.bottom;
	}

	MoveBehavior::ClipResult MoveBehavior::SetValidObjectRect() {
		ClipResult clipRes{false, false};
		if (!isSuitableSize()) {
			//width truncate
			if (m_ObjectRect.left < m_ClientRect.left) {
				m_ObjectRect.left = m_ClientRect.left;
				m_ObjectRect.right = m_ObjectRect.left + m_ObjectRectWidth;
				clipRes.isClipedLeftRightSide = true;
			}
			else if (m_ObjectRect.right > m_ClientRect.right) {
				m_ObjectRect.right = m_ClientRect.right;
				m_ObjectRect.left = m_ObjectRect.right - m_ObjectRectWidth;
				clipRes.isClipedLeftRightSide = true;
			}
			//height truncate
			if (m_ObjectRect.top < m_ClientRect.top) {
				m_ObjectRect.top = m_ClientRect.top;
				m_ObjectRect.bottom = m_ObjectRect.top + m_ObjectRectHeight;
				clipRes.isClipedTopBtmSide = true;
			}
			else if (m_ObjectRect.bottom > m_ClientRect.bottom) {
				m_ObjectRect.bottom = m_ClientRect.bottom;
				m_ObjectRect.top = m_ObjectRect.bottom - m_ObjectRectHeight;
				clipRes.isClipedTopBtmSide = true;
			}
		}
		return clipRes;
	}



	///////////////////////   MouseMoveBehavior   ////////////////////////////

	MouseMoveBehavior::MouseMoveBehavior(RECT clientRect, RECT objectRect) :MoveBehavior{clientRect,objectRect} {}

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


	////////////////  AutoMoveBehaviour   //////////////////

	AutoMoveBehavior::AutoMoveBehavior(RECT clientRect, RECT objectRect): MoveBehavior{ clientRect,objectRect } {
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
			m_horizontalSpeed = (m_horizontalSpeed>0)? -m_maxHorizontalSpeed * horizontalMultiplyer: m_maxHorizontalSpeed * horizontalMultiplyer;
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
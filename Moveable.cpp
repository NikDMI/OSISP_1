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

	MouseMoveBehavior::MouseMoveBehavior(RECT clientRect, RECT objectRect) :MoveBehavior{clientRect,objectRect} {

	}

	void MouseMoveBehavior::CaptureMouseCoord() {
		isCapture = true;
		GetCursorPos(&m_captureMousePosition);
		m_startCaptureObjectRect = m_ObjectRect;
	}

	void MouseMoveBehavior::UncaptureMouseCoord() {
		isCapture = false;
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
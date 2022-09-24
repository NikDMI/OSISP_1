#include "Moveable.h"

namespace LAB1 {
	MoveBehavior::MoveBehavior(RECT clientRect, RECT objectRect) :m_ClientRect{ clientRect }, m_ObjectRect{objectRect} {
		CheckObjectRect(objectRect);
		if (!isSuitableSize()) throw ExceptionBadSize{};
	}

	MoveBehavior::~MoveBehavior() {
	}

	bool MoveBehavior::isSuitableSize() {
		return m_ObjectRect.left>=m_ClientRect.left && m_ObjectRect.top>=m_ClientRect.top &&
			m_ObjectRect.right<=m_ClientRect.right && m_ObjectRect.bottom<=m_ClientRect.bottom;
	}

	void MoveBehavior::CheckObjectRect(RECT objectRect) {
		uint32_t rectHeight = objectRect.bottom - objectRect.top;
		uint32_t rectWidth = objectRect.right - objectRect.left;
		uint32_t windowHeight = m_ClientRect.bottom - m_ClientRect.top;
		uint32_t windowWidth = m_ClientRect.right - m_ClientRect.left;
		if (rectHeight <= 0 || rectWidth <= 0 || rectHeight> windowHeight || rectWidth>windowWidth) throw ExceptionBadArgs{};
		m_ObjectRectHeight = rectHeight;
		m_ObjectRectWidth = rectWidth;
		m_ObjectRect = objectRect;
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

	void MoveBehavior::SetObjectRect(RECT rect) {
		CheckObjectRect(rect);
		SetValidObjectRect();
	}

	void MoveBehavior::SetClientRect(RECT rect) {
		RECT tempRect = this->m_ClientRect;
		this->m_ClientRect = rect;
		try {
			CheckObjectRect(this->m_ObjectRect);
		}
		catch (...) {
			this->m_ClientRect = tempRect;
			throw;
		}
	}

}
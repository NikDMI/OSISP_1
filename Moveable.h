#pragma once
#include "Config.h"
#include <Windows.h>

namespace LAB1 {
	class ExceptionBadSize :public Exception {
	public:
		ExceptionBadSize() :Exception(L"Несоответствие размеров объекта и клиентской области") {};
	};


	/// 
	/// This class is used to move object's main point corresponding to some phisics laws
	/// 
	class MoveBehavior {
	private:
		bool isSuitableSize();
		uint32_t m_ObjectRectWidth;
		uint32_t m_ObjectRectHeight;
	protected:
		RECT m_ClientRect;
		RECT m_ObjectRect;
		struct ClipResult {
			bool isClipedLeftRightSide:1;
			bool isClipedTopBtmSide:1;
		};
		ClipResult SetValidObjectRect();
	public:
		MoveBehavior(RECT clientRect, RECT objectRect);
		virtual ~MoveBehavior();
		virtual void RefreshRectCoords() = 0;
		virtual const RECT& GetObjectRect() const { return m_ObjectRect; }
	};


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
		//void Move(int32_t dx, int32_t dy);
	};

}
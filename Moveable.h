#pragma once
#include "Config.h"
#include <Windows.h>
#include "Timer.h"

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
		void CheckObjectRect(RECT objectRect);
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
		const RECT& GetObjectRect() const { return m_ObjectRect; }
		void SetObjectRect(RECT rect);
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
	};

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
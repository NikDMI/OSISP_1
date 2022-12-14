#include <Windows.h>
#include <string>
#include "Painter.h"
#include "Moveable.h"
#include "MoveBehavior/MouseMoveBehaviour.h"
#include "MoveBehavior/AutoMoveBehaviour.h"
#include "MoveBehavior/StaticMoveBehaviour.h"
#include "Sprites/RectangleSprite.h"
#include "Sprites/ImageSprite.h"
#include "Timer.h"
#include "Sprite.h"
#include <vector>

using namespace LAB1;

const std::wstring CLASS_NAME = L"Class1";
const INT KEY_MOVE_OFFSET = 5;
const double WHEEL_SENSETIVE = 40; //number of pixels to "rotate" wheel


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdLine) {
	InitApplication();
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME.c_str();
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(CLASS_NAME.c_str(), L"OSISP_1", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, 0);
	ShowWindow(hWnd, SW_SHOW);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		
	}
	return 0;
}

const UINT objectW = 300;
const UINT objectH = 200;
const RECT startObjectRect{ 0,0,objectW,objectH };
bool isCapture = false;

class ExampleFactory {
private:
	Painter* m_painter = nullptr;
	MoveBehavior* m_currentMoveBehavior;
	HWND m_hWnd;

	std::vector<Sprite*> m_sprites{ 2 };
	Sprite* m_currentSprite;

	enum MoveBehaviors:char {Mouse = 0, AutoMove, StaticMove};
	std::vector<MoveBehavior*> m_moveBehaviors{3};
	Timer m_timer{};
	const double MAX_SLEEP_TIME_IN_SECONDS = 1.000f;
	bool m_isStaticMove = false;
	LONGLONG m_staticMoveStartTime;

	const std::wstring TEST_IMAGE_FILE = L"Picture.png";

	void ChangeMoveBehaviour(MoveBehaviors index) {
		MoveBehavior* lastMoveBehaviour = m_currentMoveBehavior;
		m_currentMoveBehavior = m_moveBehaviors[index];
		m_currentMoveBehavior->SetObjectRect(lastMoveBehaviour->GetObjectRect());

		if (dynamic_cast<AutoMoveBehavior*>(lastMoveBehaviour)) {
			((AutoMoveBehavior*)lastMoveBehaviour)->StopMoving();
		}else if (dynamic_cast<MouseMoveBehavior*>(lastMoveBehaviour)) {
			((MouseMoveBehavior*)lastMoveBehaviour)->UncaptureMouseCoord();
		}
		
		switch (index) {
		case MoveBehaviors::AutoMove:
			((AutoMoveBehavior*)m_currentMoveBehavior)->StartMoving();
			break;
		case MoveBehaviors::Mouse:
			((MouseMoveBehavior*)m_currentMoveBehavior)->CaptureMouseCoord();
			break;
		}
		if (index != MoveBehaviors::StaticMove) m_isStaticMove = false;
	}

	void ConfigDrawingParams() {
		if (m_isStaticMove) {
			m_timer.Tick();
			if (m_timer - m_staticMoveStartTime > MAX_SLEEP_TIME_IN_SECONDS) {
				m_isStaticMove = false;
				ChangeMoveBehaviour(MoveBehaviors::AutoMove);
			}
		}
	}

public:
	enum class PainterType {D2D1_Painter};
	enum Sprites:char {Image = 0, Rectangle};

	ExampleFactory(HWND hWnd, PainterType painterType = PainterType::D2D1_Painter) :m_hWnd{hWnd} {
		//Set painter object
		switch (painterType) {

		case PainterType::D2D1_Painter:
			m_painter = new PainterD2D1{ hWnd };
			break;
		}
		//Add move behaviours (create factory?)
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		m_moveBehaviors[MoveBehaviors::Mouse] = new MouseMoveBehavior(clientRect, startObjectRect);
		m_moveBehaviors[MoveBehaviors::AutoMove] = new AutoMoveBehavior(clientRect, startObjectRect);
		m_moveBehaviors[MoveBehaviors::StaticMove] = new StaticMoveBehaviour(clientRect, startObjectRect);
		((AutoMoveBehavior*)m_moveBehaviors[MoveBehaviors::AutoMove])->StartMoving();
		m_currentMoveBehavior = m_moveBehaviors[MoveBehaviors::AutoMove];
		//Add sprites
		m_sprites[Sprites::Rectangle] = new RectangleSprite(m_painter, startObjectRect, { 0.7,0.3,0.6,1.0 });
		m_sprites[Sprites::Image] = new ImageSprite(m_painter, startObjectRect, TEST_IMAGE_FILE);
		m_currentSprite = m_sprites[Sprites::Image];
		//Start timer
		m_timer.Reset();
	};

	void CaptureMouse() {
		ChangeMoveBehaviour(MoveBehaviors::Mouse);
	}

	void ReleaseCapture() {
		ChangeMoveBehaviour(MoveBehaviors::AutoMove);
		m_painter->InvalidateDrawArea();
	}

	void StaticMoveObject(int dx, int dy) {
		if (!m_isStaticMove) {
			m_isStaticMove = true;
			ChangeMoveBehaviour(MoveBehaviors::StaticMove);
		}
		m_timer.Tick();
		m_staticMoveStartTime = m_timer.GetCurrentCount();
		((StaticMoveBehaviour*)m_currentMoveBehavior)->SetOffsetValues(dx, dy);
		m_painter->InvalidateDrawArea();
	}

	void SetSprite(Sprites sprite) {
		m_currentSprite = m_sprites[sprite];
		m_painter->InvalidateDrawArea();
	}

	void ResizeClientRect(RECT rect) {
		m_painter->Resize(rect.right - rect.left, rect.bottom - rect.top);
		for (auto moveBehaviour : m_moveBehaviors) {
			try {
				moveBehaviour->SetClientRect(rect);
			}
			catch (...) {
				return;
			}
		}
	}

	void Draw() {
		ConfigDrawingParams();
		m_painter->StartDraw();

		m_currentMoveBehavior->RefreshRectCoords();
		m_currentSprite->SetSpriteRect(m_currentMoveBehavior->GetObjectRect());
		m_currentSprite->Draw();

		m_painter->EndDraw();
		m_painter->InvalidateDrawArea();
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	double wheelCount;
	RECT clientRect;
	static ExampleFactory* applicationFactory;

	switch (uMsg) {
	case WM_CREATE:
		applicationFactory = new ExampleFactory(hWnd);
		break;

	case WM_SIZE:
		clientRect = RECT{ 0,0,LOWORD(lParam), HIWORD(lParam) };
		applicationFactory->ResizeClientRect(clientRect);
		//painter->Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		applicationFactory->Draw();
		break;

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		applicationFactory->CaptureMouse();
		isCapture = true;
		break;

	case WM_MOUSEMOVE:
		if (isCapture) {
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONUP:
		applicationFactory->ReleaseCapture();
		isCapture = false;
		ReleaseCapture();
		break;

	case WM_CHAR:
		switch (wParam) {
		case 'D':
		case 'd':
			applicationFactory->StaticMoveObject(KEY_MOVE_OFFSET, 0);
			break;
		case 'W':
		case 'w':
			applicationFactory->StaticMoveObject(0, -KEY_MOVE_OFFSET);
			break;
		case 'S':
		case 's':
			applicationFactory->StaticMoveObject(0, KEY_MOVE_OFFSET);
			break;
		case 'A':
		case 'a':
			applicationFactory->StaticMoveObject(-KEY_MOVE_OFFSET, 0);
			break;
		case '1':
			applicationFactory->SetSprite(ExampleFactory::Sprites::Image);
			break;
		case '2':
			applicationFactory->SetSprite(ExampleFactory::Sprites::Rectangle);
			break;
		}
		break;

	case WM_MOUSEWHEEL:
		wheelCount = ((double)GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA)*WHEEL_SENSETIVE;
		if (LOWORD(wParam) == MK_SHIFT) {
			applicationFactory->StaticMoveObject(wheelCount, 0);
		}
		else {
			applicationFactory->StaticMoveObject(0, wheelCount);
		}
		break;

	case WM_ERASEBKGND:
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
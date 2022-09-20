#include <Windows.h>
#include <string>
#include "Painter.h"
#include "Moveable.h"
#include "Sprite.h"
#include <vector>

using namespace LAB1;

const std::wstring CLASS_NAME = L"Class1";

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

	std::vector<Sprite*> m_sprites;

	enum MoveBehaviors:char {Mouse = 0, AutoMove};
	std::vector<MoveBehavior*> m_moveBehaviors{2};
public:
	enum class PainterType {D2D1_Painter};
	enum Sprites:char {Picture = 0};

	ExampleFactory(HWND hWnd, PainterType painterType = PainterType::D2D1_Painter) :m_hWnd{hWnd} {
		switch (painterType) {

		case PainterType::D2D1_Painter:
			m_painter = new PainterD2D1{ hWnd };
			break;
		}
		//add move behaviours
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		m_moveBehaviors[MoveBehaviors::Mouse] = new MouseMoveBehavior(clientRect, startObjectRect);
		m_moveBehaviors[MoveBehaviors::AutoMove] = new AutoMoveBehavior(clientRect, startObjectRect);
		((AutoMoveBehavior*)m_moveBehaviors[MoveBehaviors::AutoMove])->StartMoving();
		m_currentMoveBehavior = m_moveBehaviors[MoveBehaviors::AutoMove];
		//add sprites
		//m_sprites.push_back(new )
	};

	void CaptureMouse() {
		if (dynamic_cast<AutoMoveBehavior*>(m_currentMoveBehavior)) {
			((AutoMoveBehavior*)m_currentMoveBehavior)->StopMoving();
		}
		MoveBehavior* lastMoveBehaviour = m_currentMoveBehavior;
		m_currentMoveBehavior = m_moveBehaviors[MoveBehaviors::Mouse];
		m_currentMoveBehavior->SetObjectRect(lastMoveBehaviour->GetObjectRect());
		((MouseMoveBehavior*)m_currentMoveBehavior)->CaptureMouseCoord();
	}

	void ReleaseCapture() {
		if (dynamic_cast<MouseMoveBehavior*>(m_currentMoveBehavior)) {
			((MouseMoveBehavior*)m_currentMoveBehavior)->UncaptureMouseCoord();
		}
		MoveBehavior* lastMoveBehaviour = m_currentMoveBehavior;
		m_currentMoveBehavior = m_moveBehaviors[MoveBehaviors::AutoMove];
		m_currentMoveBehavior->SetObjectRect(lastMoveBehaviour->GetObjectRect());
		((AutoMoveBehavior*)m_currentMoveBehavior)->StartMoving();
		m_painter->InvalidateDrawArea();
	}

	void Draw() {
		m_painter->StartDraw();
		m_painter->SetBrushColor({ 0.5,0.2,0.3,1.0 });

		m_currentMoveBehavior->RefreshRectCoords();
		m_painter->Rectangle(m_currentMoveBehavior->GetObjectRect());
		//m_painter->DrawImage(bmp, m_currentMoveBehavior->GetObjectRect());
		m_painter->EndDraw();
		m_painter->InvalidateDrawArea();
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Painter* painter;
	static BITMAP_HANDLE bmp;
	static MoveBehavior* currentMoveBehavior;
	static MouseMoveBehavior* mouseMoveBehavior;
	RECT clientRect;

	static ExampleFactory* applicationFactory;

	switch (uMsg) {
	case WM_CREATE:
		painter = new PainterD2D1{ hWnd };
		bmp = painter->LoadImageFromFile(L"Picture.png");
		GetClientRect(hWnd, &clientRect);
		mouseMoveBehavior = new MouseMoveBehavior(clientRect, startObjectRect);
		currentMoveBehavior = mouseMoveBehavior;

		applicationFactory = new ExampleFactory(hWnd);
		break;

	case WM_SIZE:
		painter->Resize(LOWORD(lParam), HIWORD(lParam));
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
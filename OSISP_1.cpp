#include <Windows.h>
#include <string>
#include "Painter.h"
#include "Moveable.h"

using namespace LAB1;

const std::wstring CLASS_NAME = L"Class1";

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdLine) {
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

const UINT objectW = 100;
const UINT objectH = 100;
const RECT startObjectRect{ 0,0,objectW,objectH };
bool isCapture = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Painter* painter;
	static BITMAP_HANDLE bmp;
	static MoveBehavior* currentMoveBehavior;
	static MouseMoveBehavior* mouseMoveBehavior;
	RECT clientRect;

	switch (uMsg) {
	case WM_CREATE:
		painter = new PainterD2D1{ hWnd };
		bmp = painter->LoadImageFromFile(L"Picture.png");
		GetClientRect(hWnd, &clientRect);
		mouseMoveBehavior = new MouseMoveBehavior(clientRect, startObjectRect);
		currentMoveBehavior = mouseMoveBehavior;
		break;

	case WM_SIZE:
		painter->Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		painter->StartDraw();
		painter->SetBrushColor({ 0.5,0.2,0.3,1.0 });
		painter->Rectangle({ 0,0,100,100 });

		currentMoveBehavior->RefreshRectCoords();
		painter->DrawImage(bmp, currentMoveBehavior->GetObjectRect());
		painter->EndDraw();
		break;

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		currentMoveBehavior = mouseMoveBehavior;
		mouseMoveBehavior->CaptureMouseCoord();
		isCapture = true;
		break;

	case WM_MOUSEMOVE:
		if (isCapture) {
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONUP:
		currentMoveBehavior = mouseMoveBehavior;/////
		mouseMoveBehavior->UncaptureMouseCoord();
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
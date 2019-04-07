#include "UiWindow.h"

// InitializeNativeWindowObject создаёт и инициализирует нативный объект окна
bool Shan0xEngineUI::CUiWindowChild::InitializeNativeWindowObject()
{
	RECT _rc{0, 0, this->m_Width, this->m_Height};

	if (this->m_bBorders)
		AdjustWindowRect(&_rc, WS_OVERLAPPEDWINDOW, false);

	if (this->m_bCentered)
	{
		this->m_xPos = (GetSystemMetrics(SM_CXSCREEN) - _rc.right) / 2;
		this->m_yPos = (GetSystemMetrics(SM_CYSCREEN) - _rc.bottom) / 2;
	}

	if (this->m_hWnd = CreateWindowEx(
		0,
		this->m_windowClassName.c_str(),
		this->m_windowTitle.c_str(),
		(this->m_bBorders ? WS_OVERLAPPEDWINDOW : WS_POPUP | WS_CAPTION),
		this->m_xPos, this->m_yPos, _rc.right, _rc.bottom, this->m_hParentHwnd, nullptr, GetModuleHandle(nullptr), this
	); !this->m_hWnd)
		return false;

	SetWindowLongPtr(this->m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG>(Shan0xEngineUI::CUiWindowChild::StaticWindowProc));

	ShowWindow(this->m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(this->m_hWnd);
	return true;
}

// StaticWindowProc статическая процедура обработки сообщений окна
LRESULT Shan0xEngineUI::CUiWindowChild::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Shan0xEngineUI::CUiWindowChild* pWindow;
	if (uMsg == WM_NCCREATE)
	{
		pWindow = static_cast<Shan0xEngineUI::CUiWindowChild*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow)))
		{
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		pWindow = reinterpret_cast<Shan0xEngineUI::CUiWindowChild*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pWindow)
	{
		pWindow->m_hWnd = hWnd;
		return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WindowProc основная процедура обработки сообщений окна
LRESULT Shan0xEngineUI::CUiWindowChild::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			if (this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback != nullptr)
				this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback(dynamic_cast<Shan0xEngineUI::CUiWindowChild*>(this));
		}
		return 0;

		case WM_PAINT:
		{
			if (this->m_sWindowCallbacks.PAINT_WINDOW.fnPaintCallback != nullptr)
				this->m_sWindowCallbacks.PAINT_WINDOW.fnPaintCallback(dynamic_cast<Shan0xEngineUI::CUiWindow*>(this));
		}
		return 0;

		case WM_SIZE:
		{
			if (this->m_sWindowCallbacks.RESIZE_WINDOW.fnResizeCallback != nullptr)
				this->m_sWindowCallbacks.RESIZE_WINDOW.fnResizeCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));

			this->m_Width = static_cast<int>(LOWORD(lParam));
			this->m_Height = static_cast<int>(HIWORD(lParam));
		}
		return 0;

		case WM_MOVE:
		{
			if (this->m_sWindowCallbacks.MOVE_WINDOW.fnMoveCallback != nullptr)
				this->m_sWindowCallbacks.MOVE_WINDOW.fnMoveCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));

			this->m_xPos = static_cast<int>(LOWORD(lParam));
			this->m_yPos = static_cast<int>(HIWORD(lParam));
		}
		return 0;

		case WM_LBUTTONDOWN:
		{
			if (this->m_bDragMove)
			{
				ReleaseCapture();
				SendMessage(hWnd, WM_SYSCOMMAND, 61458, 0);
			}

			if (this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_DOWN.fnMlButtonDownCallback != nullptr)
				this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_DOWN.fnMlButtonDownCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
		}
		return 0;

		case WM_LBUTTONUP:
		{
			if (this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_UP.fnMlButtonUpCallback != nullptr)
				this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_UP.fnMlButtonUpCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
		}
		return 0;

		case WM_RBUTTONDOWN:
		{
			if (this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_DOWN.fnMrButtonDownCallback != nullptr)
				this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_DOWN.fnMrButtonDownCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
		}
		return 0;

		case WM_RBUTTONUP:
		{
			if (this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_UP.fnMrButtonUpCallback != nullptr)
				this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_UP.fnMrButtonUpCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
		}
		return 0;

		case WM_MOUSEMOVE:
		{
			if (this->m_sWindowCallbacks.MOUSE_MOVE.fnMMoveCallback != nullptr)
				this->m_sWindowCallbacks.MOUSE_MOVE.fnMMoveCallback(this, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
		}
		return 0;

		case WM_DESTROY:
		{
			if (this->m_sWindowCallbacks.DESTROY_WINDOW.fnDestroyCallback != nullptr)
				this->m_sWindowCallbacks.DESTROY_WINDOW.fnDestroyCallback(dynamic_cast<Shan0xEngineUI::CUiWindowChild*>(this));
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindowChild::CUiWindowChild(const std::wstring && windowTitle, const bool && windowCenter, const bool && windowBorder, const bool && windowDragMove, CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback, const int && width, const int && height, const int && xPos, const int && yPos)
{
	this->m_windowTitle = windowTitle;
	this->m_bCentered = windowCenter;
	this->m_bBorders = windowBorder;
	this->m_bDragMove = windowDragMove;
	this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback = pfnCreateWindowCallback;
	this->m_Width = width;
	this->m_Height = height;
	this->m_xPos = xPos;
	this->m_yPos = yPos;
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindowChild::CUiWindowChild(windowOptions * sCreateOptions)
{
	if (sCreateOptions != nullptr)
	{
		this->m_windowTitle = sCreateOptions->windowTitle;
		this->m_bCentered = sCreateOptions->windowCenter;
		this->m_bBorders = sCreateOptions->windowBorder;
		this->m_bDragMove = sCreateOptions->windowDragMove;
		this->m_bShadow = sCreateOptions->windowShadow;
		this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback = sCreateOptions->pfnCreateWindowCallback;
		this->m_Width = sCreateOptions->width;
		this->m_Height = sCreateOptions->height;
		this->m_xPos = sCreateOptions->xPos;
		this->m_yPos = sCreateOptions->yPos;
	}
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindowChild::CUiWindowChild(windowOptions & sCreateOptions)
{
	this->m_windowTitle = sCreateOptions.windowTitle;
	this->m_bCentered = sCreateOptions.windowCenter;
	this->m_bBorders = sCreateOptions.windowBorder;
	this->m_bDragMove = sCreateOptions.windowDragMove;
	this->m_bShadow = sCreateOptions.windowShadow;
	this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback = sCreateOptions.pfnCreateWindowCallback;
	this->m_Width = sCreateOptions.width;
	this->m_Height = sCreateOptions.height;
	this->m_xPos = sCreateOptions.xPos;
	this->m_yPos = sCreateOptions.yPos;
}

// DTOR(Деструктор)
Shan0xEngineUI::CUiWindowChild::~CUiWindowChild()
{
}

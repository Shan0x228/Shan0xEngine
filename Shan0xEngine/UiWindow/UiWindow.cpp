#include "UiWindow.h"

// InitializeNativeWindowObject создаёт и инициализирует нативный объект окна
bool Shan0xEngineUI::CUiWindow::InitializeNativeWindowObject()
{
	// Инициализация DirectX
	if (this->m_bGraphicsCore)
	{
		if (this->m_pGraphicsCore == nullptr)
		{
			this->m_pGraphicsCore = new Shan0xEngineUI::CGraphicsCore{};
			if (!this->m_pGraphicsCore->CreateDeviceResources())
			{
				delete this->m_pGraphicsCore;
				this->m_pGraphicsCore = nullptr;
				return false;
			}
		}
	}

	RECT _rc{0, 0, this->m_Width, this->m_Height};
	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) };
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject((this->m_bShadow ? BLACK_BRUSH : WHITE_BRUSH)));
	_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hInstance = GetModuleHandle(nullptr);
	_wc.lpfnWndProc = Shan0xEngineUI::CUiWindow::StaticWindowProc;
	_wc.lpszClassName = this->m_windowClassName.c_str();
	_wc.lpszMenuName = nullptr;
	_wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&_wc))
		return false;

	if (this->m_bBorders)
		AdjustWindowRect(&_rc, WS_OVERLAPPEDWINDOW, false);

	if (this->m_bCentered)
	{
		this->m_xPos = (GetSystemMetrics(SM_CXSCREEN) - _rc.right) / 2;
		this->m_yPos = (GetSystemMetrics(SM_CYSCREEN) - _rc.bottom) / 2;
	}

	if (this->m_hWnd = CreateWindowEx(
		0,
		_wc.lpszClassName,
		this->m_windowTitle.c_str(),
		(this->m_bBorders ? WS_OVERLAPPEDWINDOW : WS_POPUP | WS_CAPTION),
		this->m_xPos, this->m_yPos,
		_rc.right, _rc.bottom,
		nullptr, nullptr, _wc.hInstance, this
	); !this->m_hWnd)
		return false;

	return true;
}

// StaticWindowProc статическая процедура обработки сообщений окна
LRESULT Shan0xEngineUI::CUiWindow::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Shan0xEngineUI::CUiWindow* pWindow;
	if (uMsg == WM_NCCREATE)
	{
		pWindow = static_cast<Shan0xEngineUI::CUiWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow)))
		{
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		pWindow = reinterpret_cast<Shan0xEngineUI::CUiWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pWindow)
	{
		pWindow->m_hWnd = hWnd;
		return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WindowProc основная процедура обработки сообщений окна
LRESULT Shan0xEngineUI::CUiWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			if (this->m_bShadow)
			{
				MARGINS pMargins{ 0, 0, 0, 1 };
				DwmExtendFrameIntoClientArea(hWnd, &pMargins);

				// Force the system to recalculate NC area (making it send WM_NCCALCSIZE).
				SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
			}

			if (this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback != nullptr)
				this->m_sWindowCallbacks.CREATE_WINDOW.fnCreateCallback(dynamic_cast<Shan0xEngineUI::CUiWindow*>(this));
		}
		return 0;

		case WM_PAINT:
		{
			BeginPaint(hWnd, &this->m_pS);

			if (this->m_sWindowCallbacks.PAINT_WINDOW.fnPaintCallback != nullptr)
				this->m_sWindowCallbacks.PAINT_WINDOW.fnPaintCallback(dynamic_cast<Shan0xEngineUI::CUiWindow*>(this));
			
			EndPaint(hWnd, &this->m_pS);
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

		case WM_NCCALCSIZE:
		{
			if (this->m_bShadow)
			{
				// Returning 0 from the message when wParam is TRUE removes the standard
				// frame, but keeps the window shadow.
				if (wParam == TRUE)
				{
					SetWindowLong(hWnd, DWL_MSGRESULT, 0);
					return true;
				}
			}
		}
		return false;

		case WM_DESTROY:
		{
			this->m_vChildWindows.clear();

			if (this->m_sWindowCallbacks.DESTROY_WINDOW.fnDestroyCallback != nullptr)
				this->m_sWindowCallbacks.DESTROY_WINDOW.fnDestroyCallback(dynamic_cast<Shan0xEngineUI::CUiWindow*>(this));
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindow::CUiWindow(const std::wstring && windowTitle, const bool && windowCenter, const bool && windowBorder, const bool && windowDragMove, CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback, const int && width, const int && height, const int && xPos, const int && yPos)
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

	this->InitializeNativeWindowObject();
}

// Close закрывает окно
void Shan0xEngineUI::CUiWindow::Close()
{
	PostQuitMessage(EXIT_SUCCESS);
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindow::CUiWindow(rootWindowOptions * sCreateOptions)
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
		this->m_bGraphicsCore = sCreateOptions->uiGraphicsCore;

		this->InitializeNativeWindowObject();
	}
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiWindow::CUiWindow(rootWindowOptions & sCreateOptions)
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
	this->m_bGraphicsCore = sCreateOptions.uiGraphicsCore;

	this->InitializeNativeWindowObject();
}

// Run запускает цикл обработки сообщений окна(отображает окно на экране)
int Shan0xEngineUI::CUiWindow::Run()
{
	MSG _msg{};

	ShowWindow(this->m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(this->m_hWnd);

	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return static_cast<int>(_msg.wParam);
}

// AddChildWindow создаёт дочернее окно
void Shan0xEngineUI::CUiWindow::AddChildWindow(Shan0xEngineUI::CUiWindowChild & childWindow)
{
	childWindow.m_hParentHwnd = this->m_hWnd;
	childWindow.m_bGraphicsCore = this->m_bGraphicsCore;
	childWindow.InitializeNativeWindowObject();
	this->m_vChildWindows.push_back(&childWindow);
}

// AddChildWindow создаёт дочернее окно
void Shan0xEngineUI::CUiWindow::AddChildWindow(Shan0xEngineUI::CUiWindowChild * childWindow)
{
	if (childWindow != nullptr)
	{
		childWindow->m_hParentHwnd = this->m_hWnd;
		childWindow->m_bGraphicsCore = this->m_bGraphicsCore;
		childWindow->InitializeNativeWindowObject();
		this->m_vChildWindows.push_back(childWindow);
	}
}

// DTOR(Деструктор)
Shan0xEngineUI::CUiWindow::~CUiWindow()
{
	if (this->m_pGraphicsCore != nullptr && this->m_bGraphicsCore)
	{
		delete this->m_pGraphicsCore;
		this->m_pGraphicsCore = nullptr;
	}

	DestroyWindow(this->m_hWnd);
	UnregisterClass(this->m_windowClassName.c_str(), GetModuleHandle(nullptr));
}

// GetWindowHandle возвращает дескриптор окна
HWND Shan0xEngineUI::CUiWindowBase::GetWindowHandle()
{
	return this->m_hWnd;
}

// SetTitle устанавливает заголовок окна
void Shan0xEngineUI::CUiWindowBase::SetTitle(const std::wstring && windowTitle)
{
	this->m_windowTitle = windowTitle;
	SetWindowText(this->m_hWnd, windowTitle.c_str());
}

// SetTitle устанавливает заголовок окна
void Shan0xEngineUI::CUiWindowBase::SetTitle(const std::wstring & windowTitle)
{
	this->m_windowTitle = windowTitle;
	SetWindowText(this->m_hWnd, windowTitle.c_str());
}

// GetGraphicsCore возвращает указатель на объект графического ядра
const Shan0xEngineUI::CGraphicsCore * Shan0xEngineUI::CUiWindowBase::GetGraphicsCore()
{
	return this->m_pGraphicsCore;
}

// GetSize возвращает размеры окна
std::pair<int, int> Shan0xEngineUI::CUiWindowBase::GetSize()
{
	return { this->m_Width, this->m_Height };
}

// GetPos возвращает координаты окна
std::pair<int, int> Shan0xEngineUI::CUiWindowBase::GetPos()
{
	return {this->m_xPos, this->m_yPos};
}

// Close закрывает окно
void Shan0xEngineUI::CUiWindowBase::Close()
{
	DestroyWindow(this->m_hWnd);
}

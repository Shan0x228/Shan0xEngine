#pragma once

#pragma comment(lib, "dwmapi")

#include <Windows.h>
#include <dwmapi.h>
#include <vector>
#include <functional>
#include <string>
#include "GraphicsCore.h"
#include "UiRenderer.h"

namespace Shan0xEngineUI
{
	// CALLBACK_TYPE перечисление, содержащее тип callback'а
	enum class CALLBACK_TYPE
	{
		DESTROY_WINDOW,						// закрытие  окна
		RESIZE_WINDOW,						// изменение размеров окна
		MOVE_WINDOW,						// движение окна
		PAINT_WINDOW,						// рисование в окне

		MOUSE_LEFT_BUTTON_DOWN,				// нажатие ЛКМ
		MOUSE_LEFT_BUTTON_UP,				// отжатие ЛКМ
		MOUSE_RIGHT_BUTTON_DOWN,			// нажатие ПКМ
		MOUSE_RIGHT_BUTTON_UP,				// отжатие ПКМ
		MOUSE_MOVE,							// движение мышью
	};

	class CUiWindowChild;
	class CUiWindow;

	// CUiWindowBase базовый класс окон
	class CUiWindowBase
	{
	// Секция с общедоступными методами
	public:
		// AddWindowCallbackFunction добавляет callback к событию окна
		template<CALLBACK_TYPE T, typename Fptr>
		void AddWindowCallbackFunction(Fptr fnCallback)
		{
			if constexpr (T == CALLBACK_TYPE::DESTROY_WINDOW)
			{
				this->m_sWindowCallbacks.DESTROY_WINDOW.fnDestroyCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::RESIZE_WINDOW)
			{
				this->m_sWindowCallbacks.RESIZE_WINDOW.fnResizeCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOVE_WINDOW)
			{
				this->m_sWindowCallbacks.MOVE_WINDOW.fnMoveCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::PAINT_WINDOW)
			{
				this->m_sWindowCallbacks.PAINT_WINDOW.fnPaintCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOUSE_LEFT_BUTTON_DOWN)
			{
				this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_DOWN.fnMlButtonDownCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOUSE_LEFT_BUTTON_UP)
			{
				this->m_sWindowCallbacks.MOUSE_LEFT_BUTTON_UP.fnMlButtonUpCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOUSE_RIGHT_BUTTON_DOWN)
			{
				this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_DOWN.fnMrButtonDownCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOUSE_RIGHT_BUTTON_UP)
			{
				this->m_sWindowCallbacks.MOUSE_RIGHT_BUTTON_UP.fnMrButtonUpCallback = fnCallback;
			}
			else if constexpr (T == CALLBACK_TYPE::MOUSE_MOVE)
			{
				this->m_sWindowCallbacks.MOUSE_MOVE.fnMMoveCallback = fnCallback;
			}
		}

		// GetWindowHandle возвращает дескриптор окна
		virtual HWND GetWindowHandle();
		
		// SetTitle устанавливает заголовок окна
		virtual void SetTitle(const std::wstring && windowTitle);
		virtual void SetTitle(const std::wstring & windowTitle);

		// GetGraphicsCore возвращает указатель на объект графического ядра
		virtual const CGraphicsCore* GetGraphicsCore();

		// GetSize возвращает размеры окна
		virtual std::pair<int, int> GetSize();
		// GetPos возвращает координаты окна
		virtual std::pair<int, int> GetPos();

		// Close закрывает окно
		virtual void Close();

		// fnCreateWindowCallback удобное определение для функции callback'а
		using fnCreateWindowCallback = std::function<void(Shan0xEngineUI::CUiWindowBase* sender)>;

	// Секция с приватными виртуальными методами
	private:
		// InitializeNativeWindowObject создаёт и инициализирует нативный объект окна
		virtual bool InitializeNativeWindowObject() = 0;

		// WindowProc основная процедура обработки сообщений окна
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	// Секция с наследуемыми полями
	protected:
		const std::wstring m_windowClassName{ L"Shan0xEngineUI_Window" };		// поле содержит текст класса окна
		std::wstring m_windowTitle{};											// поле содержит текст заголовка окна
		int m_xPos{}, m_yPos{};													// поле содержит координаты окна
		int m_Width{}, m_Height{};												// поле содержит размеры окна
		HWND m_hWnd{};															// поле содержит дескриптор окна

		bool m_bCentered{false};
		bool m_bBorders{ true };
		bool m_bDragMove{false};
		bool m_bShadow{ false };

		bool m_bGraphicsCore{ false };
		CGraphicsCore* m_pGraphicsCore{ nullptr };

		// структура содержит callback'и окна
		struct WINDOW_CALLBACKS
		{
			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender)> fnCreateCallback{};
			} CREATE_WINDOW;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender)> fnDestroyCallback{};
			} DESTROY_WINDOW;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int width, int height)> fnResizeCallback{};
			} RESIZE_WINDOW;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int x, int y)> fnMoveCallback{};
			} MOVE_WINDOW;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender)> fnPaintCallback{};
			} PAINT_WINDOW;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int cursorX, int cursorY)> fnMlButtonDownCallback{};
			} MOUSE_LEFT_BUTTON_DOWN;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int cursorX, int cursorY)> fnMlButtonUpCallback{};
			} MOUSE_LEFT_BUTTON_UP;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int cursorX, int cursorY)> fnMrButtonDownCallback{};
			} MOUSE_RIGHT_BUTTON_DOWN;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int cursorX, int cursorY)> fnMrButtonUpCallback{};
			} MOUSE_RIGHT_BUTTON_UP;

			struct
			{
				std::function<void(Shan0xEngineUI::CUiWindowBase* sender, int cursorX, int cursorY)> fnMMoveCallback{};
			} MOUSE_MOVE;

		} m_sWindowCallbacks;
	};

	// *windowOptions структура с параметрами создания окна
	struct windowOptions
	{
		std::wstring windowTitle{L"Window"};
		bool windowCenter{false};
		bool windowBorder{true};
		bool windowDragMove{false};
		bool windowShadow{ false };
		CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback{nullptr};
		int width{600};
		int height{600};
		int xPos{0};
		int yPos{0};
	};
	struct rootWindowOptions
	{
		std::wstring windowTitle{ L"Window" };
		bool windowCenter{ false };
		bool windowBorder{ true };
		bool windowDragMove{ false };
		bool windowShadow{ false };
		bool uiGraphicsCore{ false };
		CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback{ nullptr };
		int width{ 600 };
		int height{ 600 };
		int xPos{ 0 };
		int yPos{ 0 };
	};

	// CUiWindow класс родительского окна
	class CUiWindow : public CUiWindowBase
	{
	// Секция с приватными методами
	private:
		// InitializeNativeWindowObject создаёт и инициализирует нативный объект окна
		virtual bool InitializeNativeWindowObject();

		// StaticWindowProc статическая процедура обработки сообщений окна
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// WindowProc основная процедура обработки сообщений окна
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Секция с приватными полями
	private:
		std::vector<CUiWindowChild*> m_vChildWindows{};							// поле содержит вектор(динамический массив) объектов дочерних окон
		PAINTSTRUCT m_pS{};

	// Секция с общедоступными методами
	public:
		// CTOR(Конструктор)
		CUiWindow(
			const std::wstring && windowTitle = { L"Form" },
			const bool && windowCenter = false,
			const bool && windowBorder = true,
			const bool && windowDragMove = false,
			CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback = nullptr,
			const int && width = {600},
			const int && height = {600},
			const int && xPos = {0},
			const int && yPos = {0}
		);

		// Close закрывает окно
		virtual void Close();

		// CTOR(Конструктор)
		CUiWindow(rootWindowOptions * sCreateOptions = nullptr);
		CUiWindow(rootWindowOptions & sCreateOptions);

		// Run запускает цикл обработки сообщений окна(отображает окно на экране)
		int Run();

		// AddChildWindow создаёт дочернее окно
		void AddChildWindow(Shan0xEngineUI::CUiWindowChild & childWindow);
		void AddChildWindow(Shan0xEngineUI::CUiWindowChild * childWindow = nullptr);

		// DTOR(Деструктор)
		~CUiWindow();
	};

	// CUiWindowChild класс дочерних окон
	class CUiWindowChild : public CUiWindowBase
	{
	// Секция с приватными методами
	private:
		// InitializeNativeWindowObject создаёт и инициализирует нативный объект окна
		virtual bool InitializeNativeWindowObject();

		// StaticWindowProc статическая процедура обработки сообщений окна
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// WindowProc основная процедура обработки сообщений окна
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Секция с приватными полями
	private:
		HWND m_hParentHwnd{};													// поле содержит дескриптор родительского окна
		PAINTSTRUCT m_pS{};

	// Секция с общедоступными методами
	public:
		// CTOR(Конструктор)
		CUiWindowChild(
			const std::wstring && windowTitle = { L"ChildForm" },
			const bool && windowCenter = false,
			const bool && windowBorder = true,
			const bool && windowDragMove = false,
			CUiWindowBase::fnCreateWindowCallback pfnCreateWindowCallback = nullptr,
			const int && width = { 600 },
			const int && height = { 600 },
			const int && xPos = { 0 },
			const int && yPos = { 0 }
		);

		// CTOR(Конструктор)
		CUiWindowChild(windowOptions * sCreateOptions = nullptr);
		CUiWindowChild(windowOptions & sCreateOptions);

		// DTOR(Деструктор)
		~CUiWindowChild();

		friend class CUiWindow;
	};
}
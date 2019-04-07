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
	// CALLBACK_TYPE ������������, ���������� ��� callback'�
	enum class CALLBACK_TYPE
	{
		DESTROY_WINDOW,						// ��������  ����
		RESIZE_WINDOW,						// ��������� �������� ����
		MOVE_WINDOW,						// �������� ����
		PAINT_WINDOW,						// ��������� � ����

		MOUSE_LEFT_BUTTON_DOWN,				// ������� ���
		MOUSE_LEFT_BUTTON_UP,				// ������� ���
		MOUSE_RIGHT_BUTTON_DOWN,			// ������� ���
		MOUSE_RIGHT_BUTTON_UP,				// ������� ���
		MOUSE_MOVE,							// �������� �����
	};

	class CUiWindowChild;
	class CUiWindow;

	// CUiWindowBase ������� ����� ����
	class CUiWindowBase
	{
	// ������ � �������������� ��������
	public:
		// AddWindowCallbackFunction ��������� callback � ������� ����
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

		// GetWindowHandle ���������� ���������� ����
		virtual HWND GetWindowHandle();
		
		// SetTitle ������������� ��������� ����
		virtual void SetTitle(const std::wstring && windowTitle);
		virtual void SetTitle(const std::wstring & windowTitle);

		// GetGraphicsCore ���������� ��������� �� ������ ������������ ����
		virtual const CGraphicsCore* GetGraphicsCore();

		// GetSize ���������� ������� ����
		virtual std::pair<int, int> GetSize();
		// GetPos ���������� ���������� ����
		virtual std::pair<int, int> GetPos();

		// Close ��������� ����
		virtual void Close();

		// fnCreateWindowCallback ������� ����������� ��� ������� callback'�
		using fnCreateWindowCallback = std::function<void(Shan0xEngineUI::CUiWindowBase* sender)>;

	// ������ � ���������� ������������ ��������
	private:
		// InitializeNativeWindowObject ������ � �������������� �������� ������ ����
		virtual bool InitializeNativeWindowObject() = 0;

		// WindowProc �������� ��������� ��������� ��������� ����
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	// ������ � ������������ ������
	protected:
		const std::wstring m_windowClassName{ L"Shan0xEngineUI_Window" };		// ���� �������� ����� ������ ����
		std::wstring m_windowTitle{};											// ���� �������� ����� ��������� ����
		int m_xPos{}, m_yPos{};													// ���� �������� ���������� ����
		int m_Width{}, m_Height{};												// ���� �������� ������� ����
		HWND m_hWnd{};															// ���� �������� ���������� ����

		bool m_bCentered{false};
		bool m_bBorders{ true };
		bool m_bDragMove{false};
		bool m_bShadow{ false };

		bool m_bGraphicsCore{ false };
		CGraphicsCore* m_pGraphicsCore{ nullptr };

		// ��������� �������� callback'� ����
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

	// *windowOptions ��������� � ����������� �������� ����
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

	// CUiWindow ����� ������������� ����
	class CUiWindow : public CUiWindowBase
	{
	// ������ � ���������� ��������
	private:
		// InitializeNativeWindowObject ������ � �������������� �������� ������ ����
		virtual bool InitializeNativeWindowObject();

		// StaticWindowProc ����������� ��������� ��������� ��������� ����
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// WindowProc �������� ��������� ��������� ��������� ����
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ������ � ���������� ������
	private:
		std::vector<CUiWindowChild*> m_vChildWindows{};							// ���� �������� ������(������������ ������) �������� �������� ����
		PAINTSTRUCT m_pS{};

	// ������ � �������������� ��������
	public:
		// CTOR(�����������)
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

		// Close ��������� ����
		virtual void Close();

		// CTOR(�����������)
		CUiWindow(rootWindowOptions * sCreateOptions = nullptr);
		CUiWindow(rootWindowOptions & sCreateOptions);

		// Run ��������� ���� ��������� ��������� ����(���������� ���� �� ������)
		int Run();

		// AddChildWindow ������ �������� ����
		void AddChildWindow(Shan0xEngineUI::CUiWindowChild & childWindow);
		void AddChildWindow(Shan0xEngineUI::CUiWindowChild * childWindow = nullptr);

		// DTOR(����������)
		~CUiWindow();
	};

	// CUiWindowChild ����� �������� ����
	class CUiWindowChild : public CUiWindowBase
	{
	// ������ � ���������� ��������
	private:
		// InitializeNativeWindowObject ������ � �������������� �������� ������ ����
		virtual bool InitializeNativeWindowObject();

		// StaticWindowProc ����������� ��������� ��������� ��������� ����
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// WindowProc �������� ��������� ��������� ��������� ����
		virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ������ � ���������� ������
	private:
		HWND m_hParentHwnd{};													// ���� �������� ���������� ������������� ����
		PAINTSTRUCT m_pS{};

	// ������ � �������������� ��������
	public:
		// CTOR(�����������)
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

		// CTOR(�����������)
		CUiWindowChild(windowOptions * sCreateOptions = nullptr);
		CUiWindowChild(windowOptions & sCreateOptions);

		// DTOR(����������)
		~CUiWindowChild();

		friend class CUiWindow;
	};
}
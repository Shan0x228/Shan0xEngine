/*-------
  ALPHA!
-------*/

#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include "GraphicsCore.h"

namespace Shan0xEngineUI
{
	class CUiRenderer
	{
	private:
		// CreateGraphicsResources инициализирует ресурсы DirectX
		virtual bool CreateGraphicsResources();
		// DiscardGraphicsResources высвобождает ресурсы DirectX
		virtual void DiscardGraphicsResources();

	private:
		ID2D1HwndRenderTarget* m_pRT{};
		CGraphicsCore* m_pGraphicsCore{};
		HWND m_hWnd{};
		int m_Width{}, m_Height{};

	public:
		// CTOR(Конструктор)
		CUiRenderer(const CGraphicsCore * pGraphicsCore, HWND hWnd, int width, int height);

		// Init инициализирует ресурсы
		bool Init();

		// GetRT возвращает RenderTarget
		ID2D1HwndRenderTarget* GetRT();

		// DTOR(Деструктор)
		~CUiRenderer();
	};
}
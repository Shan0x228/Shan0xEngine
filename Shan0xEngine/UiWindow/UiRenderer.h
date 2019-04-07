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
		// CreateGraphicsResources �������������� ������� DirectX
		virtual bool CreateGraphicsResources();
		// DiscardGraphicsResources ������������ ������� DirectX
		virtual void DiscardGraphicsResources();

	private:
		ID2D1HwndRenderTarget* m_pRT{};
		CGraphicsCore* m_pGraphicsCore{};
		HWND m_hWnd{};
		int m_Width{}, m_Height{};

	public:
		// CTOR(�����������)
		CUiRenderer(const CGraphicsCore * pGraphicsCore, HWND hWnd, int width, int height);

		// Init �������������� �������
		bool Init();

		// GetRT ���������� RenderTarget
		ID2D1HwndRenderTarget* GetRT();

		// DTOR(����������)
		~CUiRenderer();
	};
}
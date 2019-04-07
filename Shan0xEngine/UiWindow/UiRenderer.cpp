#include "UiRenderer.h"

// CreateGraphicsResources инициализирует ресурсы DirectX
bool Shan0xEngineUI::CUiRenderer::CreateGraphicsResources()
{
	if (this->m_pGraphicsCore->hr = this->m_pGraphicsCore->m_pD2D1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(this->m_hWnd, 
		D2D1::SizeU(static_cast<UINT32>(this->m_Width), static_cast<UINT32>(this->m_Height))), &this->m_pRT); !SUCCEEDED(this->m_pGraphicsCore->hr))
		return false;

	return true;
}

// DiscardGraphicsResources высвобождает ресурсы DirectX
void Shan0xEngineUI::CUiRenderer::DiscardGraphicsResources()
{
	this->m_pGraphicsCore->SafeRelease(&this->m_pRT);
}

// CTOR(Конструктор)
Shan0xEngineUI::CUiRenderer::CUiRenderer(const CGraphicsCore * pGraphicsCore, HWND hWnd, int width, int height)
	: m_hWnd(hWnd), m_Width(width), m_Height(height)
{
	this->m_pGraphicsCore = const_cast<CGraphicsCore*>(pGraphicsCore);
}

// Init инициализирует ресурсы
bool Shan0xEngineUI::CUiRenderer::Init()
{
	return this->CreateGraphicsResources();
}

// GetRT возвращает RenderTarget
ID2D1HwndRenderTarget * Shan0xEngineUI::CUiRenderer::GetRT()
{
	return this->m_pRT;
}

// DTOR(Деструктор)
Shan0xEngineUI::CUiRenderer::~CUiRenderer()
{
	this->DiscardGraphicsResources();
}

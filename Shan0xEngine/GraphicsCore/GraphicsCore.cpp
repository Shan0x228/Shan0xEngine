#include "GraphicsCore.h"

// CreateDeviceResources инициализирует DirectX
bool Shan0xEngineUI::CGraphicsCore::CreateDeviceResources()
{
	if (this->hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->m_pD2D1Factory); !SUCCEEDED(this->hr))
		return false;

	if (this->hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&this->m_pDWriteFactory)); !SUCCEEDED(this->hr))
		return false;

	return true;
}

// DiscardDeviceResources высвобождает DirectX
void Shan0xEngineUI::CGraphicsCore::DiscardDeviceResources()
{
	this->SafeRelease(&this->m_pDWriteFactory);
	this->SafeRelease(&this->m_pD2D1Factory);
}

// CTOR(Конструктор)
Shan0xEngineUI::CGraphicsCore::CGraphicsCore()
	:m_pD2D1Factory(nullptr)
{
}

// DTOR(Деструктор)
Shan0xEngineUI::CGraphicsCore::~CGraphicsCore()
{
	this->DiscardDeviceResources();
}
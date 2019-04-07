#pragma once
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <d2d1.h>
#include <dwrite.h>

namespace Shan0xEngineUI
{
	class CUiWindow;
	class CUiWindowChild;
	class CUiRenderer;
	class CGraphicsCore
	{
	// Секция с приватными методами
	private:
		template<typename T>
		inline void SafeRelease(T ** ppT)
		{
			if (*ppT != nullptr)
			{
				(*ppT)->Release();
				*ppT = nullptr;
			}
		}

		// CreateDeviceResources инициализирует DirectX
		bool CreateDeviceResources();

		// DiscardDeviceResources высвобождает DirectX
		void DiscardDeviceResources();

	// Секция с приватными полями
	private:
		HRESULT hr{};
		ID2D1Factory* m_pD2D1Factory{};
		IDWriteFactory* m_pDWriteFactory{};

	// Секция с общедоступными методами
	public:
		// CTOR(Конструктор)
		CGraphicsCore();

		// DTOR(Деструктор)
		~CGraphicsCore();

		friend class CUiWindow;
		friend class CUiWindowChild;
		friend class CUiRenderer;
	};
}
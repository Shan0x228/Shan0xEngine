#include "UiWindow.h"
using namespace Shan0xEngineUI;

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	rootWindowOptions options{};
	options.uiGraphicsCore = true;
	options.windowCenter = true;
	options.windowDragMove = true;
	options.windowBorder = false;
	options.windowShadow = true;
	CUiWindow RootWindow{options};
	CUiRenderer rndr{ RootWindow.GetGraphicsCore(), RootWindow.GetWindowHandle(), RootWindow.GetSize().first, RootWindow.GetSize().second };
	if (!rndr.Init())
		return EXIT_FAILURE;

	auto rt = rndr.GetRT();

	RootWindow.AddWindowCallbackFunction<CALLBACK_TYPE::PAINT_WINDOW>([&rt](CUiWindowBase* sender)
	{
		rt->BeginDraw();
		rt->Clear(D2D1::ColorF(D2D1::ColorF::Crimson));

		rt->EndDraw();
	});

	RootWindow.AddWindowCallbackFunction<CALLBACK_TYPE::MOUSE_RIGHT_BUTTON_DOWN>([](CUiWindowBase* sender, int, int)
	{
		sender->Close();
	});

	windowOptions optns{};
	CUiWindowChild* chld = new CUiWindowChild{optns};
	chld->AddWindowCallbackFunction<CALLBACK_TYPE::MOUSE_RIGHT_BUTTON_DOWN>([](CUiWindowBase* sender, int, int)
	{
		sender->Close();
	});

	RootWindow.AddChildWindow(chld);

	return RootWindow.Run();
}
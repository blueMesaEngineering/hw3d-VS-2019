//#include "App.h"
//
//App::App()
//	:
//	wnd(800, 600, L"The Donkey Fart Box - Now with RAMEN!")
//{}
//
//int App::DoStuff()
//{
//	//MSG msg;
//	//bool gResult;
//	//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
//	//{
//	//	// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
//	//	TranslateMessage(&msg);
//	//	DispatchMessage(&msg);
//
//	//	DoFrame();
//	//}
//
//	//// check if GetMessage call itself borked
//	//if (gResult == -1)
//	//{
//	//	throw CHWND_LAST_EXCEPT();
//	//}
//
//	//// wParam here is the value passed to PostQuitMessage
//	//return msg.wParam;
//
//	while (true)
//	{
//		if (const auto ecode = Window::ProcessMessages())
//		{
//			// if return optional has value, means we're
//			return *ecode;
//		}
//		DoFrame();
//	}
//}
//
//void App::DoFrame()
//{
//	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
//	wnd.Gfx().ClearBuffer(c, c, 1.0f);
//	//wnd.Gfx().DrawTestTriangle(
//	//	timer.Peek()
//	//	, 0.0f
//	//	, 0.0f
//	//);
//	wnd.Gfx().DrawTestTriangle(
//		timer.Peek()
//		, wnd.mouse.GetPosX() / 400.0f - 1.0f
//		, wnd.mouse.GetPosY() / -300.0f + 1.0f
//	);
//	wnd.Gfx().EndFrame();
//}
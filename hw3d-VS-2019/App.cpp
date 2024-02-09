#include "App.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include "TexturePreprocessor.h"
#include <shellapi.h>

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(1280, 720, L"The Donkey Fart Box - Now with RAMEN!"),
	light(wnd.Gfx())
{
	// Makeshift cli for doing some preprocessing bullshit (so many hacks here)
	if (this->commandLine != "")
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-objnorm")
		{
			const std::wstring pathInWide = pArgs[2];
			TexturePreprocessor::FlipYAllNormalMapsInObj(
				std::string(pathInWide.begin(), pathInWide.end())
				);
			throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		}
	}
	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({ 12.0f, 0.0f, 0.0f });
	//gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	//nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 400.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	//wall.Draw(wnd.Gfx());
	//tp.Draw(wnd.Gfx());
	//nano.Draw(wnd.Gfx());
	//gobber.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());
	sponza.Draw(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f, 0.0f, dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt, 0.0f, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f, 0.0f, -dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt, 0.0f, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f, dt, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f, -dt, 0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}

	// ImGui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	//gobber.ShowWindow(wnd.Gfx(), "gobber");
	//wall.ShowWindow(wnd.Gfx(), "Wall");
	//tp.SpawnControlWindow(wnd.Gfx());
	//nano.ShowWindow(wnd.Gfx(), "Nano");
	sponza.ShowWindow(wnd.Gfx(), "Sponza");

	// Present
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

App::~App()
{}


int App::DoStuff()
{
	while (true)
	{
		// Process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}
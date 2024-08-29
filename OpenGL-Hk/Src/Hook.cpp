#include <MinHook.h>
#include <gl/GL.h>

#include "Hook.h"
#include "GUI.h"

typedef BOOL(__stdcall* TWglSwapBuffers)(HDC hDc);

static bool is_init{ false };
static HWND wnd_handle{ nullptr };
static WNDPROC origin_wndproc{ nullptr };
void* p_swap_buffers{ nullptr };
TWglSwapBuffers origin_wglSwapBuffers{ nullptr };

static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static BOOL __stdcall wglSwapBuffers(HDC hDc);

//
// Yönetim fonksiyonlarý
//

bool Hook::init()
{
	if (is_init)
		return false;

	// WndProc
	{
		wnd_handle = FindWindowA("LWJGL", nullptr);

		if (!wnd_handle)
		{
			wnd_handle = FindWindowA("GLFW30", nullptr);

			if (!wnd_handle)
			{
				printf("[-] Pencere tanýmlayýcý alýnamadý\n");
				return false;
			}
		}

		origin_wndproc = (WNDPROC)SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}

	MH_Initialize();

	// wglSwapBuffers
	{
		p_swap_buffers = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");

		if (p_swap_buffers == nullptr)
		{
			printf("[-] wglSwapBuffers adresi alýnamadý\n");
			return false;
		}

		MH_CreateHook(p_swap_buffers, &wglSwapBuffers, (LPVOID*)&origin_wglSwapBuffers);
	}

	MH_EnableHook(MH_ALL_HOOKS);

	is_init = true;

	return false;
}

void Hook::shutdown()
{
	if (!is_init)
		return;

	// Hook'larý devre dýþý býrak ve kaldýr
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// Orijinal WndProc'u geri yükle
	SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)origin_wndproc);

	is_init = false;
}

//
// Get fonksiyonlarý
//

bool Hook::getIsInit()
{
	return is_init;
}

//
// Hook'lar
//

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GUI::getIsInit())
	{
		if (msg == WM_KEYDOWN && wParam == VK_INSERT)
			GUI::setDoDraw(!GUI::getDoDraw());

		if (GUI::getDoDraw() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
	}

	return CallWindowProcA(origin_wndproc, hWnd, msg, wParam, lParam);
}

BOOL __stdcall wglSwapBuffers(HDC hDc)
{
	static HGLRC origin_context{ nullptr };
	static HGLRC new_context{ nullptr };
	static bool was_init{ false };

	HWND new_wnd_handle = WindowFromDC(hDc);

	if (wnd_handle != new_wnd_handle && GUI::getIsInit() && was_init)
	{
		GUI::shutdown();
		Hook::shutdown();
		Hook::init();
		was_init = false;
	}

	if (!was_init)
	{
		origin_context = wglGetCurrentContext();

		new_context = wglCreateContext(hDc);
		if (!new_context)
		{
			printf("[-] Yeni OpenGL context'i oluþturulamadý\n");
			return origin_wglSwapBuffers(hDc);
		}
		wglMakeCurrent(hDc, new_context);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);

		GUI::init(wnd_handle);

		was_init = true;
	}
	else
	{
		wglMakeCurrent(hDc, new_context);
		GUI::draw();
	}

	wglMakeCurrent(hDc, origin_context);

	return origin_wglSwapBuffers(hDc);
}

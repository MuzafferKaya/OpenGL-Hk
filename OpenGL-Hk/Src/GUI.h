#ifndef GUI_H_
#define GUI_H_

#include <cstdio>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

namespace GUI
{
    //
    // Y�netim fonksiyonlar�
    //

    bool init(HWND wnd_handle);
    void shutdown();

    //
    // Global fonksiyonlar
    //

    void draw();

    //
    // Get & Set fonksiyonlar�
    //

    bool getIsInit();
    bool getDoDraw();

    void setDoDraw(bool new_value);
}

#endif

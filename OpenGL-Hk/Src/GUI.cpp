#include "GUI.h"

namespace GUI
{
    static bool is_init{ false }; // Ba�lang��ta false
    static bool do_draw{ true };

    //
    // Y�netim fonksiyonlar�
    //

    bool init(HWND wnd_handle)
    {
        if (is_init)
            return false; // Zaten ba�lat�ld�ysa false d�nd�r

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(wnd_handle);
        ImGui_ImplOpenGL3_Init();

        is_init = true;

        return true; // Ba�ar�yla ba�lat�ld�ysa true d�nd�r
    }

    void shutdown()
    {
        if (!is_init)
            return;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        is_init = false;
    }

    //
    // Global fonksiyonlar
    //

    void draw()
    {
        if (!do_draw)
            return;

        printf("TEST\n");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("OpenGL-Hk");
        {
            ImGui::Text("Hello, World!");
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    //
    // Get & Set fonksiyonlar�
    //

    bool getIsInit()
    {
        return is_init;
    }

    bool getDoDraw()
    {
        return do_draw;
    }

    void setDoDraw(bool new_value)
    {
        do_draw = new_value;
    }
}

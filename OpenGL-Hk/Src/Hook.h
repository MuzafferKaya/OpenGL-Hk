#ifndef HOOK_H_
#define HOOK_H_

#include <cstdio>
#include <Windows.h>

namespace Hook
{
    //
    // Yönetim fonksiyonlarý
    //

    bool init();
    void shutdown();

    //
    // Get fonksiyonlarý
    //

    bool getIsInit();
}

#endif

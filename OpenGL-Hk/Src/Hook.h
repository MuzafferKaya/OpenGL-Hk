#ifndef HOOK_H_
#define HOOK_H_

#include <cstdio>
#include <Windows.h>

namespace Hook
{
    //
    // Yönetim fonksiyonları
    //

    bool init();
    void shutdown();

    //
    // Get fonksiyonları
    //

    bool getIsInit();
}

#endif

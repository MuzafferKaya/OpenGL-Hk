#ifndef HOOK_H_
#define HOOK_H_

#include <cstdio>
#include <Windows.h>

namespace Hook
{
    //
    // Y�netim fonksiyonlar�
    //

    bool init();
    void shutdown();

    //
    // Get fonksiyonlar�
    //

    bool getIsInit();
}

#endif

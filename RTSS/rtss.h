#ifndef RTSSOVERLAY_RTSS_H
#define RTSSOVERLAY_RTSS_H

#include "../utils/include.h"
#include "structs.h"
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <intrin.h>

namespace Legend {

    class RTSS {
        LPRTSS_SHARED_MEMORY o_mem = nullptr;
        HANDLE o_handle = nullptr;
        bool o_failed = false;

        char o_osd[4096] = {};
        DWORD o_osdLen = 0;
        DWORD o_bufferOffset = 0;

        RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY GetSlot();

    public:
        RTSS();
        ~RTSS();

        auto DidFail() -> bool;

        void Begin();
        void End();

        void Text(int x, int y, DWORD color, const char* text);
        void TextF(int x, int y, DWORD color, const char* fmt, ...);
        void Box(int x, int y, int w, int h, DWORD color);
        void FilledRect(int x, int y, int w, int h, DWORD color, float value = 1.0f);
        void HLine(int x, int y, int len, DWORD color);
        void VLine(int x, int y, int len, DWORD color);
    };
}

#endif

#include "rtss.h"

namespace Legend {

    RTSS::RTSS() {
        o_handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "RTSSSharedMemoryV2");
        if (!o_handle) {
            Log::Message(Log::Error, "Failed To Open Handle To RTSSSharedMemoryV2");
            o_failed = true;
            return;
        }
        Log::Message(Log::Success, "RTSSSharedMemoryV2 Handle 0x%llx", 1, 1, o_handle);

        o_mem = reinterpret_cast<LPRTSS_SHARED_MEMORY>(
            MapViewOfFile(o_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0));

        if (!o_mem || o_mem->dwSignature != 'RTSS' || o_mem->dwVersion < 0x00020000) {
            Log::Message(Log::Error, "Failed To Map File");
            o_failed = true;
            return;
        }

        Log::Message(Log::Success, "Mapped File 0x%llx", 1, 1, o_mem);
    }

    RTSS::~RTSS() {
        if (o_mem) {
            auto* slot = GetSlot();
            if (slot) {
                memset(slot, 0, o_mem->dwOSDEntrySize);
                o_mem->dwOSDFrame++;
            }
            UnmapViewOfFile(o_mem);
        }
        if (o_handle)
            CloseHandle(o_handle);
    }

    auto RTSS::DidFail() -> bool {
        return o_failed;
    }

    RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY RTSS::GetSlot() {
        if (o_failed) return nullptr;

        for (DWORD pass = 0; pass < 2; pass++) {
            for (DWORD i = 1; i < o_mem->dwOSDArrSize; i++) {
                auto* entry = reinterpret_cast<RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY>(
                    reinterpret_cast<LPBYTE>(o_mem) + o_mem->dwOSDArrOffset + i * o_mem->dwOSDEntrySize);

                if (pass && !strlen(entry->szOSDOwner))
                    strcpy_s(entry->szOSDOwner, sizeof(entry->szOSDOwner), "Legend");

                if (!strcmp(entry->szOSDOwner, "Legend"))
                    return entry;
            }
        }
        return nullptr;
    }

    void RTSS::Begin() {
        o_osd[0] = '\0';
        o_osdLen = 0;
        o_bufferOffset = 0;
    }

    void RTSS::End() {
        if (o_failed) return;

        auto* slot = GetSlot();
        if (!slot) return;

        if (o_mem->dwVersion >= 0x0002000e) {
            if (!_interlockedbittestandset(&o_mem->dwBusy, 0)) {
                strncpy_s(slot->szOSDEx, sizeof(slot->szOSDEx), o_osd, sizeof(slot->szOSDEx) - 1);
                o_mem->dwBusy = 0;
            }
        } else {
            strncpy_s(slot->szOSDEx, sizeof(slot->szOSDEx), o_osd, sizeof(slot->szOSDEx) - 1);
        }

        o_mem->dwOSDFrame++;
    }

    void RTSS::Text(int x, int y, DWORD color, const char* text) {
        char buf[512];
        snprintf(buf, sizeof(buf), "<P=%d,%d><C=%06X>%s<C>", x, y, color & 0xFFFFFF, text);
        strncat_s(o_osd, sizeof(o_osd), buf, _TRUNCATE);
    }

    void RTSS::TextF(int x, int y, DWORD color, const char* fmt, ...) {
        char content[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(content, sizeof(content), fmt, args);
        va_end(args);
        Text(x, y, color, content);
    }

    void RTSS::Box(int x, int y, int w, int h, DWORD color) {
        char line[512];
        char hbar[128];

        int inner = w - 2;
        if (inner < 0) inner = 0;
        if (inner > 126) inner = 126;

        memset(hbar, '\xC4', inner);
        hbar[inner] = '\0';

        snprintf(line, sizeof(line), "<P=%d,%d><C=%06X>\xDA%s\xBF<C>", x, y, color & 0xFFFFFF, hbar);
        strncat_s(o_osd, sizeof(o_osd), line, _TRUNCATE);

        for (int row = 1; row < h - 1; row++) {
            char spaces[128];
            memset(spaces, ' ', inner);
            spaces[inner] = '\0';
            snprintf(line, sizeof(line), "<P=%d,%d><C=%06X>\xB3%s\xB3<C>", x, y + row, color & 0xFFFFFF, spaces);
            strncat_s(o_osd, sizeof(o_osd), line, _TRUNCATE);
        }

        if (h > 1) {
            snprintf(line, sizeof(line), "<P=%d,%d><C=%06X>\xC0%s\xD9<C>", x, y + h - 1, color & 0xFFFFFF, hbar);
            strncat_s(o_osd, sizeof(o_osd), line, _TRUNCATE);
        }
    }

    void RTSS::FilledRect(int x, int y, int w, int h, DWORD color, float value) {
        if (o_failed || o_mem->dwVersion < 0x0002000c) return;

        auto* slot = GetSlot();
        if (!slot) return;

        DWORD objSize = sizeof(RTSS_EMBEDDED_OBJECT_GRAPH) + sizeof(FLOAT);
        if (o_bufferOffset + objSize > sizeof(slot->buffer)) return;

        auto* graph = reinterpret_cast<LPRTSS_EMBEDDED_OBJECT_GRAPH>(slot->buffer + o_bufferOffset);
        graph->header.dwSignature = RTSS_EMBEDDED_OBJECT_GRAPH_SIGNATURE;
        graph->header.dwSize = objSize;
        graph->header.dwWidth = w;
        graph->header.dwHeight = h;
        graph->header.dwMargin = 0;
        graph->dwFlags = RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_BAR
                       | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FILLED
                       | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_BGND;
        graph->fltMin = 0.0f;
        graph->fltMax = 1.0f;
        graph->dwDataCount = 1;
        graph->fltData[0] = value;

        char buf[256];
        snprintf(buf, sizeof(buf), "<P=%d,%d><C=%06X><OBJ=%08X><C>", x, y, color & 0xFFFFFF, o_bufferOffset);
        strncat_s(o_osd, sizeof(o_osd), buf, _TRUNCATE);

        o_bufferOffset += objSize;
    }

    void RTSS::HLine(int x, int y, int len, DWORD color) {
        char line[256];
        char bar[128];

        if (len > 126) len = 126;
        if (len < 1) len = 1;

        memset(bar, '\xC4', len);
        bar[len] = '\0';

        snprintf(line, sizeof(line), "<P=%d,%d><C=%06X>%s<C>", x, y, color & 0xFFFFFF, bar);
        strncat_s(o_osd, sizeof(o_osd), line, _TRUNCATE);
    }

    void RTSS::VLine(int x, int y, int len, DWORD color) {
        char line[128];
        for (int i = 0; i < len; i++) {
            snprintf(line, sizeof(line), "<P=%d,%d><C=%06X>\xB3<C>", x, y + i, color & 0xFFFFFF);
            strncat_s(o_osd, sizeof(o_osd), line, _TRUNCATE);
        }
    }
}

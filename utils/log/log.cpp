#include "log.h"
#include <fstream>
#include <filesystem>

namespace Legend::Log {
    bool debug = true;

    static std::ofstream &GetLogFile() {
        static std::ofstream file;
        static bool initialized = false;
        if (!initialized) {
            initialized = true;

            char exe_path[MAX_PATH] = {};
            GetModuleFileNameA(NULL, exe_path, MAX_PATH);
            std::filesystem::path logs_dir = std::filesystem::path(exe_path).parent_path() / "logs";
            std::filesystem::create_directories(logs_dir);

            auto now = std::chrono::system_clock::now();
            auto time_t_val = std::chrono::system_clock::to_time_t(now);
            std::tm t = {};
            localtime_s(&t, &time_t_val);

            char filename[32];
            snprintf(filename, sizeof(filename), "%02d-%02d-%02d.txt", t.tm_hour, t.tm_min, t.tm_sec);

            file.open(logs_dir / filename, std::ios::out | std::ios::app);
        }
        return file;
    }

    auto Message(LogType type, const char *str,
                 bool timestamp, bool log_type,
                 ...) -> void {
        if (!debug) return;

        va_list args;
        va_start(args, log_type);

        char msg_buf[4096];
        vsnprintf(msg_buf, sizeof(msg_buf), str, args);
        va_end(args);

        const LogMeta &m = META[type];

        std::tm localTime = {};
        int ms_count = 0;

        printf("%s│%s ", m.dim_color, CLR_RESET);

        if (timestamp) {
            auto now = std::chrono::system_clock::now();
            auto time_t_val = std::chrono::system_clock::to_time_t(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            ms_count = (int) ms.count();
            localtime_s(&localTime, &time_t_val);
            printf("%s%02d:%02d:%02d%s.%s%03d%s  ",
                   CLR_TIME,
                   localTime.tm_hour, localTime.tm_min, localTime.tm_sec,
                   CLR_SEPARATOR,
                   "\033[38;2;60;60;90m",
                   ms_count,
                   CLR_RESET);
        }

        if (log_type) {
            printf("%s%s %s%-4s%s  ", m.color, m.icon, m.dim_color, m.label, CLR_RESET);
        }

        printf("%s%s%s\n", m.color, msg_buf, CLR_RESET);

        std::ofstream &log_file = GetLogFile();
        if (log_file.is_open()) {
            if (timestamp) {
                char ts[32];
                snprintf(ts, sizeof(ts), "%02d:%02d:%02d.%03d  ",
                         localTime.tm_hour, localTime.tm_min, localTime.tm_sec, ms_count);
                log_file << ts;
            }
            if (log_type) {
                char lt[16];
                snprintf(lt, sizeof(lt), "%-4s  ", m.label);
                log_file << lt;
            }
            log_file << msg_buf << "\n";
            log_file.flush();
        }
    }


    auto CreateConsole(const char *title) -> void {
        if (!debug) return;

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hConsole, &mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, mode);

        CONSOLE_FONT_INFOEX cfi = {};
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = 16;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Cascadia Mono");
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        COORD bufferSize = {500, csbi.dwSize.Y};
        SetConsoleScreenBufferSize(hConsole, bufferSize);
        SMALL_RECT windowSize = {0, 0, 499, csbi.srWindow.Bottom};
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

        CONSOLE_SCREEN_BUFFER_INFOEX csbiex = {};
        csbiex.cbSize = sizeof(csbiex);
        GetConsoleScreenBufferInfoEx(hConsole, &csbiex);
        csbiex.ColorTable[0] = RGB(8, 8, 14);
        SetConsoleScreenBufferInfoEx(hConsole, &csbiex);

        SetConsoleTitleA(title);

        HWND hwnd = GetConsoleWindow();
        if (hwnd) {
            SetWindowPos(hwnd, HWND_TOP, 100, 100, 1110, 790, SWP_SHOWWINDOW);
        }
    }

    auto PrintBanner() -> void {
        if (!debug) return;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

        const int banner_width = 54;
        int pad = (console_width - banner_width) / 2;
        if (pad < 0) pad = 0;
        std::string p(pad, ' ');
        const char *P = p.c_str();

        const char *lines[] = {
            "   ██╗     ███████╗ ██████╗ ███████╗███╗  ██╗██████╗ ",
            "   ██║     ██╔════╝██╔════╝ ██╔════╝████╗ ██║██╔══██╗",
            "   ██║     █████╗  ██║  ███╗█████╗  ██╔██╗██║██║  ██║",
            "   ██║     ██╔══╝  ██║   ██║██╔══╝  ██║╚████║██║  ██║",
            "   ███████╗███████╗╚██████╔╝███████╗██║ ╚███║██████╔╝",
            "   ╚══════╝╚══════╝ ╚═════╝ ╚══════╝╚═╝  ╚══╝╚═════╝ ",
        };

        const char *colors[] = {
            "\033[38;2;60;100;255m",
            "\033[38;2;80;130;255m",
            "\033[38;2;100;160;255m",
            "\033[38;2;120;185;255m",
            "\033[38;2;140;210;255m",
            "\033[38;2;160;230;255m",
        };

        printf("\n");
        for (int i = 0; i < 6; i++)
            printf("%s%s%s\n", P, colors[i], lines[i]);

        printf("\n%s\033[38;2;25;25;40m╔══════════════════════════════════════════════════════╗\n", P);
        printf("%s║  \033[38;2;80;80;120mRTSS Overlay  \033[38;2;50;50;80m│\033[38;2;160;160;200m  By Legend"
               "  \033[38;2;50;50;80m│\033[38;2;80;80;120m  %-12s\033[38;2;25;25;40m             ║\n", P, __DATE__);
        printf("%s╚══════════════════════════════════════════════════════╝\n\n", P);

        printf(CLR_RESET);
    }
}
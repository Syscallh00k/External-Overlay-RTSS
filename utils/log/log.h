#ifndef RTSSOVERLAY_LOG_H
#define RTSSOVERLAY_LOG_H

#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <string>
#include <iostream>

#define CLR_RESET       "\033[0m"
#define CLR_DIM         "\033[2m"
#define CLR_BOLD        "\033[1m"
#define CLR_BRACKET     "\033[38;2;50;50;70m"
#define CLR_TIME        "\033[38;2;80;80;120m"
#define CLR_SEPARATOR   "\033[38;2;30;30;50m"
#define CLR_NORMAL      "\033[38;2;120;140;180m"
#define CLR_WARNING     "\033[38;2;255;180;50m"
#define CLR_ERROR       "\033[38;2;255;70;70m"
#define CLR_SUCCESS     "\033[38;2;80;220;120m"
#define CLR_INFO        "\033[38;2;140;180;255m"
#define CLR_NORMAL_DIM  "\033[38;2;60;70;90m"
#define CLR_WARNING_DIM "\033[38;2;120;85;25m"
#define CLR_ERROR_DIM   "\033[38;2;120;35;35m"
#define CLR_SUCCESS_DIM "\033[38;2;40;110;60m"
#define CLR_INFO_DIM    "\033[38;2;70;90;130m"
#define CLR_SDK         "\033[38;2;255;140;0m"
#define CLR_SDK_DIM     "\033[38;2;120;65;0m"
#define CLR_CACHE       "\033[38;2;180;80;255m"
#define CLR_CACHE_DIM   "\033[38;2;85;35;120m"
#define CLR_THREAD      "\033[38;2;0;220;200m"
#define CLR_THREAD_DIM  "\033[38;2;0;100;90m"
#define CLR_INJECTOR     "\033[38;2;255;80;180m"
#define CLR_INJECTOR_DIM "\033[38;2;120;35;85m"
#define CLR_SHELLCODE     "\033[38;2;255;60;60m"
#define CLR_SHELLCODE_DIM "\033[38;2;110;25;25m"
#define CLR_SEGMENTS     "\033[38;2;60;210;255m"
#define CLR_SEGMENTS_DIM "\033[38;2;25;95;120m"
#define CLR_EXPORTS     "\033[38;2;255;210;0m"
#define CLR_EXPORTS_DIM "\033[38;2;115;95;0m"
#define CLR_ENTRY     "\033[38;2;0;255;140m"
#define CLR_ENTRY_DIM "\033[38;2;0;110;60m"
#define CLR_PEB      "\033[38;2;0;200;255m"
#define CLR_PEB_DIM  "\033[38;2;0;85;110m"

namespace Legend::Log {
    extern bool debug;

    enum LogType : int {
        Normal = 0,
        Warning = 1,
        Error = 2,
        Success = 3,
        Information = 4,
        SDK = 5,
        Cache = 6,
        Thread = 7,
        Injector = 8,
        Shellcode = 9,
        Segments = 10,
        Exports = 11,
        Entry = 12,
        Peb = 13,
    };

    struct LogMeta {
        const char *icon;
        const char *label;
        const char *color;
        const char *dim_color;
    };

    static const LogMeta META[] = {
        {"○", "NORM", CLR_NORMAL, CLR_NORMAL_DIM},
        {"◆", "WARN", CLR_WARNING, CLR_WARNING_DIM},
        {"✕", "FAIL", CLR_ERROR, CLR_ERROR_DIM},
        {"✓", "GOOD", CLR_SUCCESS, CLR_SUCCESS_DIM},
        {"»", "INFO", CLR_INFO, CLR_INFO_DIM},
        {"⬡", "SDK ", CLR_SDK, CLR_SDK_DIM},
        {"▣", "CACHE", CLR_CACHE, CLR_CACHE_DIM},
        {"⟳", "THRD", CLR_THREAD, CLR_THREAD_DIM},
        {"⇣", "INJ", CLR_INJECTOR, CLR_INJECTOR_DIM},
        {"»", "ASM", CLR_SHELLCODE, CLR_SHELLCODE_DIM},
        {"▤", "SEG", CLR_SEGMENTS, CLR_SEGMENTS_DIM},
        {"⊞", "EXPORT", CLR_EXPORTS, CLR_EXPORTS_DIM},
        {"⇥", "ETY", CLR_ENTRY, CLR_ENTRY_DIM},
        {"⬢", "PEB ", CLR_PEB, CLR_PEB_DIM},
    };

    auto Message(LogType type, const char *str,
                 bool timestamp = true, bool log_type = true,
                 ...) -> void;

    template<typename T>
    auto Input(LogType type, const char *prompt,
               bool timestamp = true, bool log_type = true,
               ...) -> T {
        va_list args;
        va_start(args, log_type);

        const char *colorBracket = "\033[37m";
        const char *reset = "\033[0m";

        printf("%s│%s ", META[type].dim_color, CLR_RESET);

        if (timestamp) {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) % 1000;
            std::tm localTime = {};
            localtime_s(&localTime, &time);
            printf("%s%02d:%02d:%02d%s.%s%03d%s  ",
                   CLR_TIME,
                   localTime.tm_hour, localTime.tm_min, localTime.tm_sec,
                   CLR_SEPARATOR, "\033[38;2;60;60;90m",
                   (int) ms.count(), CLR_RESET);
        }

        if (log_type) {
            const LogMeta &m = META[type];
            printf("%s%s %s%-4s%s  ", m.color, m.icon, m.dim_color, m.label, CLR_RESET);
        }

        vprintf(prompt, args);
        printf(" : ");
        fflush(stdout);
        va_end(args);

        T input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }

    auto CreateConsole(const char *title) -> void;

    auto PrintBanner() -> void;
}
#endif //RTSSOVERLAY_LOG_H
#ifndef RTSSOVERLAY_STRUCTS_H
#define RTSSOVERLAY_STRUCTS_H

#include <windows.h>

typedef struct VIDEO_CAPTURE_PARAM
{
    DWORD   dwVersion;
    char    szFilename[MAX_PATH];
    DWORD   dwFramerate;
    DWORD   dwFramesize;
    DWORD   dwFormat;
    DWORD   dwQuality;
    DWORD   dwThreads;
    BOOL    bCaptureOSD;
    DWORD   dwAudioCaptureFlags;
    DWORD   dwVideoCaptureFlagsEx;
    DWORD   dwAudioCaptureFlags2;
    DWORD   dwPrerecordSizeLimit;
    DWORD   dwPrerecordTimeLimit;
} VIDEO_CAPTURE_PARAM, *LPVIDEO_CAPTURE_PARAM;

#define OSDFLAG_UPDATED         0x00000001
#define STATFLAG_RECORD         0x00000001

#define APPFLAG_OGL             0x00000001
#define APPFLAG_DD              0x00000002
#define APPFLAG_D3D8            0x00000003
#define APPFLAG_D3D9            0x00000004
#define APPFLAG_D3D9EX          0x00000005
#define APPFLAG_D3D10           0x00000006
#define APPFLAG_D3D11           0x00000007
#define APPFLAG_D3D12           0x00000008
#define APPFLAG_D3D12AFR        0x00000009
#define APPFLAG_VULKAN          0x0000000A

#define APPFLAG_API_USAGE_MASK                  0x0000FFFF
#define APPFLAG_ARCHITECTURE_X64                0x00010000
#define APPFLAG_ARCHITECTURE_UWP                0x00020000
#define APPFLAG_PROFILE_UPDATE_REQUESTED        0x10000000

#define SCREENCAPTUREFLAG_REQUEST_CAPTURE       0x00000001
#define SCREENCAPTUREFLAG_REQUEST_CAPTURE_OSD   0x00000010

#define VIDEOCAPTUREFLAG_REQUEST_CAPTURE_START      0x00000001
#define VIDEOCAPTUREFLAG_REQUEST_CAPTURE_PROGRESS   0x00000002
#define VIDEOCAPTUREFLAG_REQUEST_CAPTURE_STOP       0x00000004
#define VIDEOCAPTUREFLAG_REQUEST_CAPTURE_MASK       0x00000007
#define VIDEOCAPTUREFLAG_REQUEST_CAPTURE_OSD        0x00000010
#define VIDEOCAPTUREFLAG_INTERNAL_RESIZE            0x00010000

#define PROCESS_PERF_COUNTER_ID_RAM_USAGE               0x00000001
#define PROCESS_PERF_COUNTER_ID_D3DKMT_VRAM_USAGE_LOCAL     0x00000100
#define PROCESS_PERF_COUNTER_ID_D3DKMT_VRAM_USAGE_SHARED   0x00000101

typedef struct RTSS_SHARED_MEMORY
{
    DWORD   dwSignature;

    DWORD   dwVersion;

    DWORD   dwAppEntrySize;
    DWORD   dwAppArrOffset;
    DWORD   dwAppArrSize;

    DWORD   dwOSDEntrySize;
    DWORD   dwOSDArrOffset;
    DWORD   dwOSDArrSize;

    DWORD   dwOSDFrame;

    LONG    dwBusy;

    DWORD   dwDesktopVideoCaptureFlags;
    DWORD   dwDesktopVideoCaptureStat[5];

    DWORD   dwLastForegroundApp;
    DWORD   dwLastForegroundAppProcessID;

    DWORD   dwProcessPerfCountersEntrySize;
    DWORD   dwProcessPerfCountersArrOffset;

    LARGE_INTEGER qwLatencyMarkerSetTimestamp;
    LARGE_INTEGER qwLatencyMarkerResetTimestamp;

    typedef struct RTSS_SHARED_MEMORY_OSD_ENTRY
    {
        char    szOSD[256];
        char    szOSDOwner[256];

        char    szOSDEx[4096];

        BYTE    buffer[262144];

        char    szOSDEx2[32768];

    } RTSS_SHARED_MEMORY_OSD_ENTRY, *LPRTSS_SHARED_MEMORY_OSD_ENTRY;

    typedef struct RTSS_SHARED_MEMORY_PROCESS_PERF_COUNTER_ENTRY
    {
        DWORD   dwID;
        DWORD   dwParam;
        DWORD   dwData;
    } RTSS_SHARED_MEMORY_PROCESS_PERF_COUNTER_ENTRY, *LPRTSS_SHARED_MEMORY_PROCESS_PERF_COUNTER_ENTRY;

    typedef struct RTSS_SHARED_MEMORY_APP_ENTRY
    {
        DWORD   dwProcessID;
        char    szName[MAX_PATH];
        DWORD   dwFlags;

        DWORD   dwTime0;
        DWORD   dwTime1;
        DWORD   dwFrames;
        DWORD   dwFrameTime;

        DWORD   dwStatFlags;
        DWORD   dwStatTime0;
        DWORD   dwStatTime1;
        DWORD   dwStatFrames;
        DWORD   dwStatCount;
        DWORD   dwStatFramerateMin;
        DWORD   dwStatFramerateAvg;
        DWORD   dwStatFramerateMax;

        DWORD   dwOSDX;
        DWORD   dwOSDY;
        DWORD   dwOSDPixel;
        DWORD   dwOSDColor;
        DWORD   dwOSDFrame;

        DWORD   dwScreenCaptureFlags;
        char    szScreenCapturePath[MAX_PATH];

        DWORD   dwOSDBgndColor;

        DWORD   dwVideoCaptureFlags;
        char    szVideoCapturePath[MAX_PATH];
        DWORD   dwVideoFramerate;
        DWORD   dwVideoFramesize;
        DWORD   dwVideoFormat;
        DWORD   dwVideoQuality;
        DWORD   dwVideoCaptureThreads;
        DWORD   dwScreenCaptureQuality;
        DWORD   dwScreenCaptureThreads;

        DWORD   dwAudioCaptureFlags;

        DWORD   dwVideoCaptureFlagsEx;

        DWORD   dwAudioCaptureFlags2;
        DWORD   dwStatFrameTimeMin;
        DWORD   dwStatFrameTimeAvg;
        DWORD   dwStatFrameTimeMax;
        DWORD   dwStatFrameTimeCount;
        DWORD   dwStatFrameTimeBuf[1024];
        DWORD   dwStatFrameTimeBufPos;
        DWORD   dwStatFrameTimeBufFramerate;

        LARGE_INTEGER qwAudioCapturePTTEventPush;
        LARGE_INTEGER qwAudioCapturePTTEventRelease;
        LARGE_INTEGER qwAudioCapturePTTEventPush2;
        LARGE_INTEGER qwAudioCapturePTTEventRelease2;

        DWORD   dwPrerecordSizeLimit;
        DWORD   dwPrerecordTimeLimit;

        LARGE_INTEGER qwStatTotalTime;
        DWORD   dwStatFrameTimeLowBuf[1024];
        DWORD   dwStatFramerate1Dot0PercentLow;
        DWORD   dwStatFramerate0Dot1PercentLow;

        DWORD   dw1Dot0PercentLowBufPos;
        DWORD   dw0Dot1PercentLowBufPos;

        DWORD   dwProcessPerfCountersFlags;
        DWORD   dwProcessPerfCountersCount;
        DWORD   dwProcessPerfCountersSamplingPeriod;
        DWORD   dwProcessPerfCountersSamplingTime;
        DWORD   dwProcessPerfCountersTimestamp;

        LARGE_INTEGER qwLatencyMarkerPresentTimestamp;

        DWORD   dwResolutionX;
        DWORD   dwResolutionY;

        ULONGLONG   qwInputSampleTime;
        ULONGLONG   qwSimStartTime;
        ULONGLONG   qwSimEndTime;
        ULONGLONG   qwRenderSubmitStartTime;
        ULONGLONG   qwRenderSubmitEndTime;
        ULONGLONG   qwPresentStartTime;
        ULONGLONG   qwPresentEndTime;
        ULONGLONG   qwDriverStartTime;
        ULONGLONG   qwDriverEndTime;
        ULONGLONG   qwOsRenderQueueStartTime;
        ULONGLONG   qwOsRenderQueueEndTime;
        ULONGLONG   qwGpuRenderStartTime;
        ULONGLONG   qwGpuRenderEndTime;
        DWORD       dwGpuActiveRenderTime;
        DWORD       dwGpuFrameTime;

        RTSS_SHARED_MEMORY_PROCESS_PERF_COUNTER_ENTRY arrPerfCounters[256];

    } RTSS_SHARED_MEMORY_APP_ENTRY, *LPRTSS_SHARED_MEMORY_APP_ENTRY;

    RTSS_SHARED_MEMORY_OSD_ENTRY arrOSD[8];
    RTSS_SHARED_MEMORY_APP_ENTRY arrApp[256];

    VIDEO_CAPTURE_PARAM autoVideoCaptureParam;

} RTSS_SHARED_MEMORY, *LPRTSS_SHARED_MEMORY;

typedef struct RTSS_EMBEDDED_OBJECT
{
    DWORD   dwSignature;
    DWORD   dwSize;
    LONG    dwWidth;
    LONG    dwHeight;
    LONG    dwMargin;
} RTSS_EMBEDDED_OBJECT, *LPRTSS_EMBEDDED_OBJECT;

#define RTSS_EMBEDDED_OBJECT_GRAPH_SIGNATURE    'GR00'

#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FILLED      1
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE    2
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMETIME    4
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_BAR          8
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_BGND         16
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_VERTICAL     32
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_MIRRORED     64
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_AUTOSCALE    128

#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_MIN               256
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_AVG               512
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_MAX               1024
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_1DOT0_PERCENT_LOW 2048
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_0DOT1_PERCENT_LOW 4096
#define RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_BAR_RANGE                   8192

#pragma warning(disable : 4200)

typedef struct RTSS_EMBEDDED_OBJECT_GRAPH
{
    RTSS_EMBEDDED_OBJECT header;
    DWORD   dwFlags;
    FLOAT   fltMin;
    FLOAT   fltMax;
    DWORD   dwDataCount;
    FLOAT   fltData[0];
} RTSS_EMBEDDED_OBJECT_GRAPH, *LPRTSS_EMBEDDED_OBJECT_GRAPH;

#pragma warning(default : 4200)

#endif //RTSSOVERLAY_STRUCTS_H
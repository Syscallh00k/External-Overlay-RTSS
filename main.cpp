#include "RTSS/rtss.h"

int main() {
    Legend::RTSS overlay;

    if (overlay.DidFail())
        return 1;

    while (true) {
        overlay.Begin();

        overlay.Text(10, 10, 0x00FF00, "Hello World");

        overlay.End();

        Sleep(1000);
    }

    return 0;
}

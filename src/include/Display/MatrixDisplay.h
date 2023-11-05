#pragma once

namespace MatrixDisplay
{

    void Init();

    void Clear(bool state = false);

    void Scan();

    void SetPixel(int X, int Y, bool state);

    void SetLine(int Y, int value);


    enum Animations
    {
        DropdownFade,
        Countdown_3
    };

    void PlayAnimation(Animations anim);
}

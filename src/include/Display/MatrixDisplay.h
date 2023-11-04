#pragma once

namespace MatrixDisplay
{

    void Init();

    void Clear();

    void Scan();

    void SetPixel(int X, int Y, bool state);

    void SetLine(int Y, int value);
}

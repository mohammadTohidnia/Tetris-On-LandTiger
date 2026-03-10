#pragma once

#include <windows.h>

#define BACK_COLOR COLORREF(RGB(240, 240, 240)) // background color (outside board)
#define BOARD_COLOR COLORREF(RGB(2, 82, 131)) // board background color
#define CHIP_COLOR COLORREF(RGB(155, 155, 155)) // chips metallic color
#define DARKER_CHIP COLORREF(RGB(125, 125, 125))
#define KNOB_COLOR COLORREF(RGB(160, 160, 160)) // chips semi-metallic color
#define JUMPER_COLOR COLORREF(RGB(18, 231, 114))
#define BLACK COLORREF(RGB(0, 0, 0))
#define ALMOST_BLACK COLORREF(RGB(50, 50, 50))
#define WHITE COLORREF(RGB(255, 255, 255))
#define RED COLORREF(RGB(255, 0, 0))
#define BORDO COLORREF(RGB(70, 15, 10))

#define LED_ON COLORREF(RGB(255, 40, 40))
#define LED_OFF COLORREF(RGB(255, 240, 180))
#define LED_COLOR(on) on ? LED_ON : LED_OFF

#define CHIP_RECT_DEFLATE 2

#define LTIMPORT __declspec( dllimport )
#define LTEXPORT __declspec( dllexport )

#define CGRect(x, y, w, h) CRect(x, y, x + w, y + h)
#include "drawing.h"
#include "globals.h"
#include "includes.h"
#include <vector>

void drawing::drawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
    D3DRECT rect = { x, y, x + w, y + h };
    g::pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void drawing::drawPixel(int x, int y, D3DCOLOR color)
{
    drawing::drawFilledRect(x, y, 1, 1, color);
}

void drawing::drawFilledRect2(int x, int y, int w, int h, D3DCOLOR color)
{
    D3DRECT rect = { x, y, w, h };
    g::pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0); // i think this is supposed to be ColorFill but i don't want to fufill the args
}

void drawing::drawLine(float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color)
{
    ID3DXLine* m_Line;

    D3DXCreateLine(g::pDevice, &m_Line);
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    m_Line->SetWidth(width);
    if (antialias) m_Line->SetAntialias(1);
    m_Line->Begin();
    m_Line->Draw(line, 2, color);
    m_Line->End();
    m_Line->Release();
}

void drawing::drawText(LPD3DXFONT font, unsigned int x, unsigned int y, D3DCOLOR fontColor, LPCSTR Message)
{
    RECT rct;
    rct.left = x;
    rct.right = 1920; // note todo dolater marker important info: make this dynamic im catchign autism
    rct.top = y;
    rct.bottom = rct.top + 200;
    font->DrawTextA(NULL, Message, -1, &rct, 0, fontColor);
}

int drawing::getTextWidth(const char* szText, LPD3DXFONT pFont) // stack overflow
{
    RECT rcRect = { 0,0,0,0 };
    if (pFont)
    {
        // calculate required rect
        pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
            D3DCOLOR_XRGB(0, 0, 0));
    }

    // return width
    return rcRect.right - rcRect.left;
}
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LPCTSTR szColorName[] =
{
	TEXT("COLOR_SCROLLBAR"),
	TEXT("COLOR_BACKGROUND"),
	TEXT("COLOR_ACTIVECAPTION"),
	TEXT("COLOR_INACTIVECAPTION"),
	TEXT("COLOR_MENU"),
	TEXT("COLOR_WINDOW"),
	TEXT("COLOR_WINDOWFRAME"),
	TEXT("COLOR_MENUTEXT"),
	TEXT("COLOR_WINDOWTEXT"),
	TEXT("COLOR_CAPTIONTEXT"),
	TEXT("COLOR_ACTIVEBORDER"),
	TEXT("COLOR_INACTIVEBORDER"),
	TEXT("COLOR_APPWORKSPACE"),
	TEXT("COLOR_HIGHLIGHT"),
	TEXT("COLOR_HIGHLIGHTTEXT"),
	TEXT("COLOR_BTNFACE"),
	TEXT("COLOR_BTNSHADOW"),
	TEXT("COLOR_GRAYTEXT"),
	TEXT("COLOR_BTNTEXT"),
	TEXT("COLOR_INACTIVECAPTIONTEXT"),
	TEXT("COLOR_BTNHIGHLIGHT"),
	TEXT("COLOR_3DDKSHADOW"),
	TEXT("COLOR_3DLIGHT"),
	TEXT("COLOR_INFOTEXT"),
	TEXT("COLOR_INFOBK"),
	TEXT("COLOR_HOTLIGHT"),
	TEXT("COLOR_GRADIENTACTIVECAPTION"),
	TEXT("COLOR_GRADIENTINACTIVECAPTION"),
	TEXT("COLOR_MENUHILIGHT"),
	TEXT("COLOR_MENUBAR"),
};

// 補色を計算
COLORREF GetComplementColor(COLORREF rgb)
{
	const int r = GetRValue(rgb);
	const int g = GetGValue(rgb);
	const int b = GetBValue(rgb);
	const int range = 60;
	const int minRange = 128 - range;
	const int maxRange = 128 + range;
	if ((minRange<r && r<maxRange)
		&& (minRange<g && g<maxRange)
		&& (minRange<b && b<maxRange))
	{
		if (r<128 && g<128 && b<128)
			rgb = 0xffffff;
		else
			rgb = 0x0;
	}
	else
		rgb = 0xffffff ^ rgb;
	return rgb;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT rect;
			GetClientRect(hWnd, &rect);
			rect.top = 0;
			rect.bottom = 24;
			for (int i = 0; i < sizeof(szColorName) / sizeof(LPCTSTR); ++i)
			{
				const COLORREF clrSystem = GetSysColor(i);
				const COLORREF clrOldBk = SetBkColor(hdc, clrSystem);
				const COLORREF clrOldText = SetTextColor(hdc, GetComplementColor(clrSystem));
				ExtTextOut(hdc, 0, rect.top, ETO_OPAQUE, &rect, szColorName[i], lstrlen(szColorName[i]), 0);
				SetBkColor(hdc, clrOldBk);
				SetTextColor(hdc, clrOldText);
				rect.top += 24;
				rect.bottom += 24;
			}
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("System Color"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

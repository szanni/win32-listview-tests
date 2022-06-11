/* Public domain. */

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#define ID_LISTVIEW 1337
#define NROWS 5
#define NCOLS 3

HINSTANCE _instance = NULL;
const char _name[] = "Simple List View";

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND lv = NULL;

	switch (uMsg) {
	case WM_CREATE:
	{
		char text[256];
		LVCOLUMNA col = {0};
		LVITEMA item = {0};

		lv = CreateWindowExA(WS_EX_CLIENTEDGE,
				WC_LISTVIEW,
				NULL,
				WS_VISIBLE | WS_CHILD | LVS_REPORT,
				0, 0, 0, 0,
				hwnd,
				(HMENU)ID_LISTVIEW,
				_instance,
				NULL);
		if (lv == NULL) {
			perror("Create list view");
			break;
		}

		SendMessage(lv, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

		col.pszText = text;
		col.mask = LVCF_TEXT | LVCF_WIDTH;
		col.cx = 120;
		for (int i = 0; i < NCOLS; ++i) {
			wsprintfA(text, "Column %d", i);
			ListView_InsertColumn(lv, i, &col);
		}

		item.pszText = text;
		item.mask = LVIF_TEXT;
		for (int irow = 0; irow < NROWS; ++irow) {
			wsprintfA(text, "Row %d", irow);
			item.iItem = irow;
			ListView_InsertItem(lv, &item);
			for (int icol = 1; icol < NCOLS; ++icol) {
				wsprintfA(text, "Cell (%d,%d)", irow, icol);
				ListView_SetItemText(lv, irow, icol, text);
			}
		}
		break;
	}
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		MoveWindow(lv, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg = {0};
	_instance = hInstance;

	/* Silence unused warning. */
	(void)hPrevInstance;
	(void)lpCmdLine;

	INITCOMMONCONTROLSEX icex = {0};
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	WNDCLASSEX wcex = {0};
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = 0;
	wcex.lpfnWndProc   = MainWndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = _name;
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassExA(&wcex)) {
		perror("Register class");
		return 0;
	}

	hwnd = CreateWindowExA(
			WS_EX_APPWINDOW,
			_name,
			_name,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (hwnd == NULL) {
		perror("Create window");
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessageA(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return msg.wParam;
}


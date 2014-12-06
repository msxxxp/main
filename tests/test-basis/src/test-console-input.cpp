#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

void KeyEventProc(KEY_EVENT_RECORD ker)
{
	console::printf(L"Key event: ");

	if (ker.bKeyDown)
		console::printf(L"key pressed\n");
	else
		console::printf(L"key released\n");
}

void MouseEventProc(MOUSE_EVENT_RECORD mer)
{
//#ifndef MOUSE_HWHEELED
//#define MOUSE_HWHEELED 0x0008
//#endif

	console::printf(L"Mouse: flags: 0x%08X butState: 0x%08X keyState: 0x%08X\t", mer.dwEventFlags, mer.dwButtonState, mer.dwControlKeyState);

	switch (mer.dwEventFlags) {
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				console::printf(L"left button\n");
			} else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
				console::printf(L"right button\n");
				mbox(L"123", L"456");
			} else if (mer.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) {
				console::printf(L"left 2 button\n");
			} else if (mer.dwButtonState == FROM_LEFT_3RD_BUTTON_PRESSED) {
				console::printf(L"left 3 button\n");
			} else if (mer.dwButtonState == FROM_LEFT_4TH_BUTTON_PRESSED) {
				console::printf(L"left 4 button\n");
			} else if (mer.dwButtonState == 0) {
				console::printf(L"button released\n");
			} else {
				console::printf(L"several button pressed\n");
			}
			break;
		case DOUBLE_CLICK:
			console::printf(L"double click\n");
			break;
		case MOUSE_HWHEELED:
			console::printf(L"horizontal mouse wheel\n");
			break;
		case MOUSE_MOVED:
			console::printf(L"mouse moved\n");
			break;
		case MOUSE_WHEELED:
			console::printf(L"vertical mouse wheel\n");
			break;
		default:
			console::printf(L"unknown\n");
			break;
	}
}

void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	console::printf(L"Resize event\n");
	console::printf(L"Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}

void test_console_input()
{
	LogAtten(L"\n");

	HANDLE hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
	console::printf(L"%S hStdin: %p\n", __PRETTY_FUNCTION__, hStdin);

	DWORD fdwSaveOldMode;
	::GetConsoleMode(hStdin, &fdwSaveOldMode);
	console::printf(L"%S mode: 0x%08X\n", __PRETTY_FUNCTION__, (unsigned)fdwSaveOldMode);

	DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	::SetConsoleMode(hStdin, fdwMode);

	DWORD cNumRead;
	INPUT_RECORD irInBuf[128];

	for (size_t counter = 0; counter < 100; ++counter) {
		if (!ReadConsoleInputW(hStdin, irInBuf, 128, &cNumRead))
			console::printf(L"ERROR: ReadConsoleInput");

		for (DWORD i = 0; i < cNumRead; ++i) {
			switch (irInBuf[i].EventType) {
				case KEY_EVENT: // keyboard input
					KeyEventProc(irInBuf[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT: // mouse input
					MouseEventProc(irInBuf[i].Event.MouseEvent);
					break;

				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
					ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT:  // disregard focus events

				case MENU_EVENT:   // disregard menu events
					break;

				default:
					console::printf(L"Unknown event type\n");
					break;
			}
		}
	}

	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
}

#include <windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")

int(__stdcall* InsertDateTime)(int x);

int MyInsertDateTime(int x) {
	MessageBox(NULL, "Но всё-таки сделаем то что вы просите :)", "Хацкед!" , MB_OK);
	return InsertDateTime(x);
}

LONG OldWndProc;

LRESULT CALLBACK NewWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case 8888:
			MessageBox(NULL, "Доп. функционал", "INJECTED!", MB_OK);
			break;
		case 9999:
			MessageBox(NULL, "Секретик! Ееее!", "SECRET!", MB_ICONINFORMATION);
			break;
		}
		break;

	}

	return CallWindowProc((WNDPROC)OldWndProc, Hwnd, Message, wParam, lParam);
}

DWORD WINAPI Injection(LPVOID)
{
	HWND hWnd = FindWindow("Notepad", NULL);
	HMENU menu = GetMenu(hWnd);
	AppendMenu(GetSubMenu(menu, 1), MF_STRING, 8888, "INJECTED");
	DrawMenuBar(hWnd);
	AppendMenu(GetSubMenu(menu, 4), MF_STRING, 9999, "SECRET");
	OldWndProc = SetWindowLong(hWnd, GWL_WNDPROC, (long)NewWndProc);
	return TRUE;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) //Decide what to do
	{
	case DLL_PROCESS_ATTACH: //On dll attach
		InsertDateTime = (int(__stdcall*)(int))DetourFunction((PBYTE)0x01006F10, (PBYTE)MyInsertDateTime);
		CreateThread(0, NULL, (LPTHREAD_START_ROUTINE)&Injection, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH: //On thread attach
		break;
	case DLL_THREAD_DETACH: //On thread detach
		break;
	case DLL_PROCESS_DETACH: //on process detach
		DetourRemove((PBYTE)0x01006F10, (PBYTE)InsertDateTime);
		break;
	case WM_COMMAND:
		break;
	}
	return TRUE;
}
#include <windows.h>

#include <stdafx.h>

static HHOOK hhookKeyb;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL WINAPI DllMain(  HINSTANCE hModule, 
							DWORD  ul_reason_for_call, 
							LPVOID lpReserved
							)
{	
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		
		{
			char buf[128];
			wsprintf(buf,"attach pid=%d tid=%d",::GetCurrentProcessId(),::GetCurrentThreadId());
			//::MessageBox(NULL,buf,buf,0);
		}
		break;

	case DLL_PROCESS_DETACH:
		{
			char buf[128];		
			wsprintf(buf,"detach pid=%d tid=%d",::GetCurrentProcessId(),::GetCurrentThreadId());
			//::MessageBox(NULL,buf,buf,0);
		}
		break;
	}
    return TRUE;
}

//keyboard hook functions

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


int count=0;

typedef void (*Proto_callback)(int value);
volatile Proto_callback hookCallbackFun=NULL;
void setCallbackFun()
{	
	int size=16;
	
	HANDLE hMapFile;
	LPCTSTR pBuf;
	char mapFileBuf[128];
	
	wsprintf(mapFileBuf,"Global\\tibiaauto-mapfile-%d",::GetCurrentProcessId());
	
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		mapFileBuf);               // name of mapping object 
	
	if (hMapFile == NULL) 
	{ 
	
		return;
	} 

	
	pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,                    
		0,                    
		size);                   
	
	if (pBuf == NULL) 
	{       
		return;
	}	
	
	CopyMemory((PVOID)&hookCallbackFun,(PVOID)pBuf, sizeof(void *));      
	
	UnmapViewOfFile(pBuf);
	
	CloseHandle(hMapFile);
	
}



extern LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char tab[255];

	if (nCode < 0)  
    return CallNextHookEx(hhookKeyb, nCode, 
        wParam, lParam);

	if((lParam & 0x80000000) == 0)
        return CallNextHookEx(hhookKeyb, nCode, 
		wParam, lParam);

	if ((count++)%2==0)
	{				
		if (wParam==0x21)
		{
			if (!hookCallbackFun)
			{
				setCallbackFun();
			}
			if (hookCallbackFun)
			{
				//::MessageBox(0,"callback fireing","",0);
				hookCallbackFun(0x21);
			}
			
		}
		if (wParam==0x22)
		{
			if (!hookCallbackFun)
			{
				setCallbackFun();
			}
			if (hookCallbackFun) hookCallbackFun(0x22);			
		}
		

		
		int threadId=::GetCurrentThreadId();
		int procId=::GetCurrentProcessId();
	
		wsprintf(tab, "wparam=%c wparam=%x lparam=%x p=%d t=%d fun=%x", wParam, wParam,lParam,procId,threadId,hookCallbackFun);
		
		//MessageBox(NULL, tab, "Wirtualny kod wci�ni�tego klawisza",MB_OK);	
		

		
	}

	return CallNextHookEx(hhookKeyb, nCode, 
        wParam, lParam);
	
}
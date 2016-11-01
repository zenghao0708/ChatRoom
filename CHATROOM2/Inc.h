#pragma once

class CCHATROOM2Dlg;

#define WM_ICON_MSG (WM_USER+150)


class CClientItem
{
public:
	CString m_strIp;
	SOCKET m_Socket;
	HANDLE hThread;
	CCHATROOM2Dlg *m_pMainWnd;
	CClientItem(){
		m_pMainWnd=NULL;
		m_Socket=INVALID_SOCKET;
		hThread=NULL;
	}


};

#define MAX_BUH_SIZE 1024			//这里不能加；号啊 ！！！！！！

DWORD WINAPI ListenThreadFunc(LPVOID pParam);
DWORD WINAPI ClientThreadProc(LPVOID lpParameter);
DWORD WINAPI ConnectThreadFunc(LPVOID pParam);
BOOL SOCKET_select(SOCKET hsocket,int nTimeOut=100,BOOL bRead=FALSE);
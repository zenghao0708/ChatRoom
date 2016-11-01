#include "stdafx.h"
#include "CHATROOM2.h"
#include "CHATROOM2Dlg.h"
#include "Inc.h"


DWORD WINAPI ConnectThreadFunc(LPVOID pParam)
{

	CCHATROOM2Dlg *pChatRoom=(CCHATROOM2Dlg*)pParam;
	ASSERT(pChatRoom!=NULL);
	pChatRoom->m_ConnectSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (pChatRoom->m_ConnectSock==INVALID_SOCKET)
	{
		pChatRoom->EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
		pChatRoom->EnableWindow(IDC_LINK_SERVER_BTN);
		AfxMessageBox(_T("新建客户端Socket失败！"));
		return FALSE;
	}
	CString strServIp;
	pChatRoom->GetDlgItemText(IDC_SERVER_IP,strServIp);
	int iPort=pChatRoom->GetDlgItemInt(IDC_SERVER_PORT);
	if (iPort<0||iPort>65535)
	{
		pChatRoom->EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
		pChatRoom->EnableWindow(IDC_LINK_SERVER_BTN);
		AfxMessageBox(_T(" 请输入合适的端口：1-65535"));
		goto _Error_End;
	}
	char szIpAddr[16]={0};
	USES_CONVERSION;
	strcpy_s(szIpAddr,16,T2A(strServIp));

	sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(iPort);
	server.sin_addr.s_addr=inet_addr(szIpAddr);
	if (connect(pChatRoom->m_ConnectSock,(struct sockaddr*)&server,sizeof(struct sockaddr))==SOCKET_ERROR)
	{
		pChatRoom->EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
		pChatRoom->EnableWindow(IDC_LINK_SERVER_BTN);
		AfxMessageBox(_T("连接失败，请重试！"));
		goto _Error_End;
	}

	pChatRoom->bIsServer=0;
	pChatRoom->ShowMessage(_T("系统消息：连接服务器成功！"));
	while (TRUE&&!(pChatRoom->bShutDown))
	{
		if (SOCKET_select(pChatRoom->m_ConnectSock))
		{
			TCHAR szBuf[MAX_BUH_SIZE]={0};
			int iRet=recv(pChatRoom->m_ConnectSock,(char*)szBuf,MAX_BUH_SIZE,0);
			if (iRet>0)
			{
				//right
				pChatRoom->ShowMessage(szBuf);
			}else
			{
				//closesocket
				pChatRoom->ShowMessage(_T("服务器停止了！请重新连接"));
				break;
			}
		}
		Sleep(500);
	}
_Error_End:
	closesocket(pChatRoom->m_ConnectSock);
	return TRUE;
}

#include "stdafx.h"
#include "CHATROOM2.h"
#include "CHATROOM2Dlg.h"


BOOL SOCKET_select(SOCKET hsocket,int nTimeOut,BOOL bRead)
{
	fd_set fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hsocket,&fdset);
	nTimeOut=nTimeOut>1000?1000:nTimeOut;
	tv.tv_sec=0;
	tv.tv_usec=nTimeOut;

	int iRet=0;
	if (bRead)
	{
		iRet=select(0,&fdset,NULL,NULL,&tv);
	}else
	{
		iRet=select(0,NULL,&fdset,NULL,&tv);
	}

	if (iRet<=0)
	{
		return FALSE;

	}else if(FD_ISSET(hsocket,&fdset))
	{
		return TRUE;
	}
	return FALSE;

}


DWORD WINAPI ClientThreadProc(LPVOID lpParameter)
{
	CString strMsg;
	CClientItem  m_ClientItem=*(CClientItem*)lpParameter;
	while (TRUE&&!(m_ClientItem.m_pMainWnd->bShutDown))
	{
		if (SOCKET_select(m_ClientItem.m_Socket,100,TRUE))
		{
			
			TCHAR Buf[MAX_BUH_SIZE]={0};
			int iRet=recv(m_ClientItem.m_Socket,(char*)Buf,MAX_BUH_SIZE,0);
			if (iRet>0)
			{
				//right
				strMsg.Format(_T("%s"),Buf);
				strMsg=_T("客户端：")+m_ClientItem.m_strIp+_T(">")+strMsg;
				m_ClientItem.m_pMainWnd->ShowMessage(strMsg);
				m_ClientItem.m_pMainWnd->SendClientsMsg(strMsg,&m_ClientItem);     //这里要注意啊 ！！！！！
			}else
			{
				//closesocket
				strMsg=_T("客户端：")+m_ClientItem.m_strIp+_T("离开了聊天室！");
				m_ClientItem.m_pMainWnd->ShowMessage(strMsg);
				m_ClientItem.m_pMainWnd->RemoveClientFromArray(m_ClientItem);
				break;
			}
		}
		Sleep(500);
	}
	return TRUE;
}


DWORD WINAPI ListenThreadFunc(LPVOID pParam)
{
	CCHATROOM2Dlg * pChatRoom=(CCHATROOM2Dlg*)pParam;
	ASSERT(pChatRoom!=NULL);
	pChatRoom->m_Listen_Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (pChatRoom->m_Listen_Socket==INVALID_SOCKET)
	{
		AfxMessageBox(_T("新建socket失败！"));
		return FALSE;
	}
	int iPort=pChatRoom->GetDlgItemInt(IDC_POTR_LISTEN);
	if (iPort<0||iPort>65535)
	{
		AfxMessageBox(_T("请输入合适的端口：1-65535"));
		goto _Error_End;
	}
	sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(iPort);
	if (bind(pChatRoom->m_Listen_Socket,(sockaddr*)&server,sizeof(sockaddr_in))==SOCKET_ERROR)
	{
		AfxMessageBox(_T("绑定端口失败！"));
		goto _Error_End;
	}
	if (listen(pChatRoom->m_Listen_Socket,5)==SOCKET_ERROR)
	{
		AfxMessageBox(_T("监听失败！"));
		goto _Error_End;
	}
	pChatRoom->bIsServer=1;


	while (TRUE&&!(pChatRoom->bShutDown))
	{
		if (SOCKET_select(pChatRoom->m_Listen_Socket,100,TRUE))
		{
			sockaddr_in client;
			int iLen=sizeof(sockaddr_in);
			SOCKET accSock=accept(pChatRoom->m_Listen_Socket,(struct sockaddr*)&client,&iLen);
			if (accSock==INVALID_SOCKET)
			{
				continue;
			}
			CClientItem tItem;
			tItem.m_Socket=accSock;
			tItem.m_strIp=inet_ntoa(client.sin_addr);
			tItem.m_pMainWnd=pChatRoom;
			INT_PTR idx=pChatRoom->m_ClientArray.Add(tItem);
			tItem.hThread=CreateThread(NULL,0,ClientThreadProc,&(pChatRoom->m_ClientArray.GetAt(idx)),CREATE_SUSPENDED,NULL);
			pChatRoom->m_ClientArray.GetAt(idx).hThread=tItem.hThread;
			ResumeThread(tItem.hThread);

			

			Sleep(100);
		}
	}


_Error_End:
	closesocket(pChatRoom->m_Listen_Socket);
	return true;

}








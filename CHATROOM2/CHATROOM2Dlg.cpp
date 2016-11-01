// CHATROOM2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CHATROOM2.h"
#include "CHATROOM2Dlg.h"
#include "Inc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCHATROOM2Dlg dialog




CCHATROOM2Dlg::CCHATROOM2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCHATROOM2Dlg::IDD, pParent)
{
	bShowAll=false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ConnectSock=INVALID_SOCKET;
	m_Listen_Socket=INVALID_SOCKET;
	m_hListenThread=NULL;
	m_hConncetThead=NULL;
	bIsServer=-1;
	bShutDown=FALSE;
	bShowAll=FALSE;
}

void CCHATROOM2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT1, m_EditMsg);
}

BEGIN_MESSAGE_MAP(CCHATROOM2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETNETBTN, &CCHATROOM2Dlg::OnBnClickedSetnetbtn)
	ON_BN_CLICKED(IDC_START_SERVER, &CCHATROOM2Dlg::OnBnClickedStartServer)
	
	ON_BN_CLICKED(IDC_LINK_SERVER_BTN, &CCHATROOM2Dlg::OnBnClickedLinkServerBtn)
	ON_BN_CLICKED(IDC_SEND_MSG, &CCHATROOM2Dlg::OnBnClickedSendMsg)
	ON_EN_CHANGE(IDC_CHAT_TXT, &CCHATROOM2Dlg::OnEnChangeChatTxt)
	ON_BN_CLICKED(IDC_STOP_CLIENT_BTN, &CCHATROOM2Dlg::OnBnClickedStopClientBtn)
	ON_BN_CLICKED(IDC_CLIENT_SET, &CCHATROOM2Dlg::OnBnClickedClientSet)
	ON_BN_CLICKED(IDC_SERVER_SET, &CCHATROOM2Dlg::OnBnClickedServerSet)
	ON_BN_CLICKED(IDC_CLOSE, &CCHATROOM2Dlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_CLIENT_OP, &CCHATROOM2Dlg::OnBnClickedClientOp)
	ON_BN_CLICKED(IDC_SERVER_OP, &CCHATROOM2Dlg::OnBnClickedServerOp)
	ON_BN_CLICKED(IDC_STOP_SERVER, &CCHATROOM2Dlg::OnBnClickedStopServer)
	ON_BN_CLICKED(IDC_MORE, &CCHATROOM2Dlg::OnBnClickedMore)
	ON_COMMAND(ID_MENU_MINISIZED, &CCHATROOM2Dlg::OnMenuMinisized)
	ON_MESSAGE(WM_ICON_MSG,OnTrayCallBackMsg)
	ON_COMMAND(ID_MENU_DISPLAY, &CCHATROOM2Dlg::OnMenuDisplay)
	ON_COMMAND(ID_MENU_CLOSE, &CCHATROOM2Dlg::OnMenuClose)
END_MESSAGE_MAP()


// CCHATROOM2Dlg message handlers

BOOL CCHATROOM2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	bShowAll=false;
	ExtendDialog(false);
	InitAllDlg();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCHATROOM2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCHATROOM2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCHATROOM2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCHATROOM2Dlg::ExtendDialog(BOOL bShow)
{

	static CRect  m_DlgRectLarge(0,0,0,0);
	static CRect  m_DlgRextSmall(0,0,0,0);
	static CRect  m_GroupRectLarge(0,0,0,0);
	static CRect  m_GroupTectSmall(0,0,0,0);
	if ( m_DlgRectLarge.IsRectNull() )
	{
		GetWindowRect(&m_DlgRectLarge);
		m_DlgRextSmall=m_DlgRectLarge;
		m_DlgRextSmall.right-=250;
		::GetWindowRect(GetDlgItem(IDC_FRAME)->GetSafeHwnd(),&m_GroupRectLarge);
		m_GroupTectSmall=m_GroupRectLarge;
		m_GroupTectSmall.right-=250;


	}

	if (bShow)
	{
		bShowAll=TRUE;
		SetWindowPos(NULL,0,0,m_DlgRectLarge.Width(),m_DlgRectLarge.Height(),SWP_NOZORDER|SWP_NOMOVE);
		::SetWindowPos(GetDlgItem(IDC_FRAME)->GetSafeHwnd(),NULL,0,0,m_GroupRectLarge.Width(),m_GroupRectLarge.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}else
	{
		bShowAll=false;
		SetWindowPos(NULL,0,0,m_DlgRextSmall.Width(),m_DlgRextSmall.Height(),SWP_NOZORDER|SWP_NOMOVE);
		::SetWindowPos(GetDlgItem(IDC_FRAME)->GetSafeHwnd(),NULL,0,0,m_GroupTectSmall.Width(),m_GroupTectSmall.Height(),SWP_NOZORDER|SWP_NOMOVE);

		//MessageBox(_T("测试成功！"));
	}



}

void CCHATROOM2Dlg::OnBnClickedSetnetbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (bShowAll)
	{
		ExtendDialog(false);
	}else
	{
		ExtendDialog(TRUE);
	}
}
void CCHATROOM2Dlg::InitAllDlg()
{
	CheckRadioButton(IDC_CLIENT_OP,IDC_SERVER_OP,IDC_CLIENT_OP);
	SetDlgItemText(IDC_SERVER_IP,_T("127.0.0.1"));
	SetDlgItemText(IDC_SERVER_PORT,_T("5566"));
	SetDlgItemText(IDC_POTR_LISTEN,_T("5566"));


	EnableWindow(IDC_STOP_CLIENT_BTN,false);
	//EnableWindow(IDC_START_SERVER,false);
	EnableWindow(IDC_STOP_SERVER,false);
	EnableWindow(IDC_POTR_LISTEN,false);
	EnableWindow(IDC_SEND_MSG,false);
	EnableWindow(IDC_START_SERVER,FALSE);
	ShowMessage(_T("程序启动成功！"));


}
BOOL CCHATROOM2Dlg::EnableWindow(UINT nID,BOOL bEnable)
{
	return GetDlgItem(nID)->EnableWindow(bEnable);
	
}

void CCHATROOM2Dlg::OnBnClickedStartServer()
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_Listen_Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_Listen_Socket==INVALID_SOCKET)
	{
		AfxMessageBox(_T("新建SOCKET失败！"));
		return;
	}
	UINT uPort=GetDlgItemInt(IDC_POTR_LISTEN);
	if (uPort<0||uPort>65535)
	{
		AfxMessageBox(_T("请输入合适的端口号！"));
		goto	_Error_End;
	}
	sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(uPort);
	if (bind(m_Listen_Socket,(sockaddr*)&server,sizeof(sockaddr_in))==SOCKET_ERROR)
	{
		AfxMessageBox(_T("绑定端口失败！"));
		goto _Error_End;
	}
	if (listen(m_Listen_Socket,5)==SOCKET_ERROR)
	{
		AfxMessageBox(_T("监听端口失败！"));
		goto _Error_End;
	}
	
	sockaddr_in client;
	int iLen=sizeof(sockaddr_in);
	SOCKET accSock=accept(m_Listen_Socket,(struct sockaddr*)&client,&iLen);
	if (accSock==INVALID_SOCKET)
	{
		AfxMessageBox(_T("接受客户端失败！"));
		goto _Error_End;
	}
_Error_End:
	closesocket(m_Listen_Socket);*/
	EnableWindow(IDC_START_SERVER,FALSE);
	EnableWindow(IDC_STOP_SERVER);
	m_hListenThread=CreateThread(NULL,0,ListenThreadFunc,this,0,NULL);
	

	


}

void CCHATROOM2Dlg::ShowMessage(CString strMsg)
{
	m_EditMsg.SetSel(-1,-1);
	m_EditMsg.ReplaceSel(strMsg+_T("\r\n"));

}
void CCHATROOM2Dlg::RemoveClientFromArray(CClientItem in_Item)
{
	for (int idx=0;idx<m_ClientArray.GetCount();idx++)
	{
		CClientItem tItem=m_ClientArray.GetAt(idx);
		if (tItem.m_Socket==in_Item.m_Socket&&
				tItem.hThread==in_Item.hThread
				&&tItem.m_strIp==in_Item.m_strIp)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}

}





void CCHATROOM2Dlg::OnBnClickedLinkServerBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableWindow(IDC_STOP_CLIENT_BTN);
	EnableWindow(IDC_LINK_SERVER_BTN,FALSE);
	m_hConncetThead=CreateThread(NULL,0,ConnectThreadFunc,this,0,NULL);
	

}

void CCHATROOM2Dlg::OnBnClickedSendMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_CHAT_TXT,strMsg);
	if (bIsServer==1)
	{
		strMsg=_T("服务器：>")+strMsg;
		ShowMessage(strMsg);
		SendClientsMsg(strMsg);
	}else if (bIsServer==0)
	{
		CString strTem=_T("本地客户端：>")+strMsg;
		ShowMessage(strTem);
		int iSend = send(m_ConnectSock,(char*)strMsg.GetBuffer(),strMsg.GetLength()*sizeof(TCHAR),0);
		strMsg.ReleaseBuffer();
	}else
	{
		AfxMessageBox(_T("请您先进入聊天室！"));

	}
	SetDlgItemText(IDC_CHAT_TXT,_T(""));


}

void CCHATROOM2Dlg::SendClientsMsg(CString strMsg,CClientItem* pNotSend)
{
	TCHAR szBuf[MAX_BUH_SIZE]={0};
	_tcscpy_s(szBuf,MAX_BUH_SIZE,strMsg);
	for (INT_PTR idx = 0;idx < m_ClientArray.GetCount(); idx++)
	{
		if (!pNotSend||pNotSend->m_Socket!=m_ClientArray.GetAt(idx).m_Socket
			||pNotSend->hThread!=m_ClientArray.GetAt(idx).hThread||
			pNotSend->m_strIp!=m_ClientArray.GetAt(idx).m_strIp)
		{
			send(m_ClientArray.GetAt(idx).m_Socket,(char*)szBuf,_tcslen(szBuf)*sizeof(TCHAR),0);
		}
	}

}
void CCHATROOM2Dlg::OnEnChangeChatTxt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_CHAT_TXT,strMsg);
	if (strMsg.IsEmpty())
	{
		EnableWindow(IDC_SEND_MSG,false);

	}else
	{
		EnableWindow(IDC_SEND_MSG);
	}

}




void CCHATROOM2Dlg::StopClient()
{
	bShowAll=true;
	DWORD dwRet=WaitForSingleObject(m_hConncetThead,1000);
	if (dwRet!=WAIT_OBJECT_0)
	{
		TerminateThread(m_hConncetThead,-1);
		closesocket(m_ConnectSock);
	}
	m_hConncetThead=NULL;
	m_ConnectSock=INVALID_SOCKET;
	bIsServer=-1;
	bShutDown=FALSE;


}

void CCHATROOM2Dlg::StopServer()
{
	UINT nCount=m_ClientArray.GetCount();
	HANDLE *m_pHandles = new HANDLE[nCount+1];
	m_pHandles[0] = m_hListenThread;
	for (int idx = 0;idx < nCount; idx++)
	{
		m_pHandles[idx+1]= m_ClientArray.GetAt(idx).hThread;
	}
	bShutDown=TRUE;
	DWORD dwRet= WaitForMultipleObjects(nCount+1,m_pHandles,TRUE,1000);
	if (dwRet!=WAIT_OBJECT_0)
	{
		for (INT_PTR i = 0; i<m_ClientArray.GetCount();i++)
		{
			TerminateThread(m_ClientArray.GetAt(i).hThread,-1);
			closesocket(m_ClientArray.GetAt(i).m_Socket);
		}
		TerminateThread(m_hListenThread,-1);
		closesocket(m_Listen_Socket);

		delete[]m_pHandles;
		m_hListenThread=NULL;
		m_Listen_Socket=INVALID_SOCKET;
		bIsServer=-1;
		bShutDown=FALSE;
		

	}

}

void CCHATROOM2Dlg::OnBnClickedStopClientBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
	EnableWindow(IDC_LINK_SERVER_BTN);
	int iRet=MessageBox(_T("真的要停止客户端吗？"),0,MB_OKCANCEL);
	if (iRet==IDOK)
	{
		StopClient();
		ShowMessage(_T("停止客户端成功！"));
		bIsServer=-1;
	}
	
}

void CCHATROOM2Dlg::OnBnClickedClientSet()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet=-1;
	if (bIsServer==1)
	{
		int iRet=MessageBox(_T("您是聊天室的服务器，如果您退出，所有客户端都要掉线！"),0,MB_OKCANCEL);
		if (iRet==IDOK)
		{
			StopServer();
		}else
		{
			CheckRadioButton(IDC_CLIENT_OP,IDC_SERVER_OP,IDC_SERVER_OP);
		}
		if (iRet==IDOK||bIsServer==-1)
		{
			EnableWindow(IDC_LINK_SERVER_BTN);
			EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
			EnableWindow(IDC_POTR_LISTEN,FALSE);
			EnableWindow(IDC_START_SERVER,FALSE);
			EnableWindow(IDC_STOP_SERVER,FALSE);
		}

	}

}

void CCHATROOM2Dlg::OnBnClickedServerSet()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet=-1;
	if (bIsServer==0)
	{
		int iRet=MessageBox(_T("您正在聊天室中，确定退出?"),_T("提示"),MB_OKCANCEL);
		if (iRet==IDOK)
		{
			StopClient();
			EnableWindow(IDC_START_SERVER);
		}else
		{
			CheckRadioButton(IDC_CLIENT_OP,IDC_SERVER_OP,IDC_CLIENT_OP);
		}
		if (iRet==IDOK||bIsServer==-1)
		{
			EnableWindow(IDC_START_SERVER);
			EnableWindow(IDC_LINK_SERVER_BTN,FALSE);
			EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
			EnableWindow(IDC_SERVER_IP,FALSE);
			EnableWindow(IDC_SERVER_PORT,FALSE);
		}

	}
}

void CCHATROOM2Dlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (bIsServer == 1)
	{
		StopServer();
		bIsServer=-1;
	}else if (bIsServer == 0)
	{
		StopClient();
		bIsServer=-1;
	}
	

	OnCancel();
}

void CCHATROOM2Dlg::OnBnClickedClientOp()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet=-1;
	if (bIsServer==1)
	{
		int iRet=MessageBox(_T("您是聊天室的服务器，如果您退出，所有客户端都要掉线！"),0,MB_OKCANCEL);
		if (iRet==IDOK)
		{
			StopServer();
			bIsServer=-1;
		}else
		{
			CheckRadioButton(IDC_CLIENT_OP,IDC_SERVER_OP,IDC_SERVER_OP);
		}
		if (iRet==IDOK||bIsServer==-1)
		{
			EnableWindow(IDC_SERVER_IP);
			EnableWindow(IDC_SERVER_PORT);
			EnableWindow(IDC_LINK_SERVER_BTN);
			EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
			EnableWindow(IDC_POTR_LISTEN,FALSE);
			EnableWindow(IDC_START_SERVER,FALSE);
			EnableWindow(IDC_STOP_SERVER,FALSE);
		}

	}
	if (bIsServer==-1)
	{
		EnableWindow(IDC_SERVER_IP);
		EnableWindow(IDC_SERVER_PORT);
		EnableWindow(IDC_LINK_SERVER_BTN);
		EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
		EnableWindow(IDC_POTR_LISTEN,FALSE);
		EnableWindow(IDC_START_SERVER,FALSE);
		EnableWindow(IDC_STOP_SERVER,FALSE);
	}
}

void CCHATROOM2Dlg::OnBnClickedServerOp()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet=-1;
	if (bIsServer==0)
	{
		int iRet=MessageBox(_T("您正在聊天室中，确定退出?"),_T("提示"),MB_OKCANCEL);
		if (iRet==IDOK)
		{
			StopClient();
			bIsServer=-1;
		}else
		{
			CheckRadioButton(IDC_CLIENT_OP,IDC_SERVER_OP,IDC_CLIENT_OP);
		}
		if (iRet==IDOK||bIsServer==-1)
		{
			EnableWindow(IDC_POTR_LISTEN);
			EnableWindow(IDC_START_SERVER);
			EnableWindow(IDC_LINK_SERVER_BTN,FALSE);
			EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
			EnableWindow(IDC_SERVER_IP,FALSE);
			EnableWindow(IDC_SERVER_PORT,FALSE);
		}

	}
	if (bIsServer==-1)
	{
		EnableWindow(IDC_POTR_LISTEN);
		EnableWindow(IDC_START_SERVER);
		EnableWindow(IDC_LINK_SERVER_BTN,FALSE);
		EnableWindow(IDC_STOP_CLIENT_BTN,FALSE);
		EnableWindow(IDC_SERVER_IP,FALSE);
		EnableWindow(IDC_SERVER_PORT,FALSE);
	}
}

void CCHATROOM2Dlg::OnBnClickedStopServer()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableWindow(IDC_START_SERVER);
	EnableWindow(IDC_STOP_SERVER,FALSE);
	int iRet=MessageBox(_T("真的要停止服务端吗？"),0,MB_OKCANCEL);
	if (iRet==IDOK)
	{
		StopServer();
		ShowMessage(_T("停止服务端成功！"));
		bIsServer=-1;
	}
}

void CCHATROOM2Dlg::OnBnClickedMore()
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint pt;
	CRect mRect;
	CMenu mMenu,*pMenu=NULL;
	GetDlgItem(IDC_MORE)->GetWindowRect(&mRect);
	pt=mRect.BottomRight();
	pt.y=mRect.top+10;
	mMenu.LoadMenu(IDR_MENU1);
	pMenu=mMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
}
BOOL CCHATROOM2Dlg::TrayMyIcon(BOOL bAdd)
{
	BOOL bRet=FALSE;
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=m_hWnd;
	tnd.uID=IDR_MAINFRAME;
	if (bAdd==TRUE)
	{
		tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
		tnd.uCallbackMessage=WM_ICON_MSG;
		tnd.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		_tcscpy_s(tnd.szTip,sizeof(tnd.szTip),_T("聊天室v1.0"));
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
		bRet=Shell_NotifyIcon(NIM_ADD,&tnd);
	}else
	{
		ShowWindow(SW_SHOWNA);
		SetForegroundWindow();
		bRet=Shell_NotifyIcon(NIM_DELETE,&tnd);

	}
	return bRet;
}

void CCHATROOM2Dlg::OnMenuMinisized()
{
	// TODO: 在此添加命令处理程序代码
	TrayMyIcon();
}

LRESULT CCHATROOM2Dlg::OnTrayCallBackMsg(WPARAM wparam,LPARAM lparam)
{
	switch(lparam)
	{
		case WM_RBUTTONUP:
			{
				CMenu mMenu,*pMenu=NULL;
				CPoint pt;
				mMenu.LoadMenu(IDR_MENU2);
				pMenu=mMenu.GetSubMenu(0);
				GetCursorPos(&pt);
				SetForegroundWindow();
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
				break;


			}
		case WM_LBUTTONDBLCLK:
			{
				ShowWindow(SW_RESTORE);
				SetForegroundWindow();
				TrayMyIcon(FALSE);
				break;
			}
		default:break;
		
	}
	return NULL;

}

void CCHATROOM2Dlg::OnMenuDisplay()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_RESTORE);
	TrayMyIcon(FALSE);
}

void CCHATROOM2Dlg::OnMenuClose()
{
	// TODO: 在此添加命令处理程序代码
	TrayMyIcon(FALSE);
	OnBnClickedClose();
}

// CHATROOM2Dlg.h : header file
//

#pragma once
#include "Inc.h"
#include "afxwin.h"
#include "Winsock2.h"
#pragma comment(lib,"Ws2_32") 
#include <windef.h>
#include <winnt.h>




// CCHATROOM2Dlg dialog
class CCHATROOM2Dlg : public CDialog
{
// Construction
public:
	CCHATROOM2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATROOM2_DIALOG};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	
	afx_msg void OnBnClickedSetnetbtn();
	afx_msg void OnBnClickedStartServer();
	afx_msg void OnBnClickedLinkServerBtn();
	afx_msg void OnBnClickedSendMsg();

	
	BOOL EnableWindow(UINT nID,BOOL bEnable=TRUE);
	BOOL bShowAll;
	int bIsServer;
	BOOL bShutDown;

	SOCKET m_Listen_Socket;
	SOCKET m_ConnectSock;
	HANDLE m_hListenThread;
	HANDLE m_hConncetThead;

	CArray <CClientItem,CClientItem>m_ClientArray;
	

	void InitAllDlg();
	void ShowMessage(CString strMsg);
	void RemoveClientFromArray(CClientItem in_Item);
	void SendClientsMsg(CString strMsg,CClientItem* pNotSend=NULL);	
	void StopClient();
	void StopServer();
	void CCHATROOM2Dlg::ExtendDialog(BOOL bShow);
	BOOL TrayMyIcon(BOOL bAdd=TRUE);
	LRESULT OnTrayCallBackMsg(WPARAM wparam,LPARAM lparam);

	
	
	
	CEdit m_EditMsg;
	afx_msg void OnBnClickedStopClientBtn();
	afx_msg void OnBnClickedClientSet();
	afx_msg void OnBnClickedServerSet();
	afx_msg void OnBnClickedClose();
	afx_msg void OnEnChangeChatTxt();
	afx_msg void OnBnClickedClientOp();
	afx_msg void OnBnClickedServerOp();
	afx_msg void OnBnClickedStopServer();
	afx_msg void OnBnClickedMore();
	afx_msg void OnMenuMinisized();
	afx_msg void OnMenuDisplay();
	afx_msg void OnMenuClose();
};

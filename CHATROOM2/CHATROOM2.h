// CHATROOM2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCHATROOM2App:
// See CHATROOM2.cpp for the implementation of this class
//

class CCHATROOM2App : public CWinApp
{
public:
	CCHATROOM2App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

};

extern CCHATROOM2App theApp;
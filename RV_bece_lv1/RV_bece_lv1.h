// RV_bece_lv1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRV_bece_lv1App:
// See RV_bece_lv1.cpp for the implementation of this class
//

class CRV_bece_lv1App : public CWinApp
{
public:
	CRV_bece_lv1App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRV_bece_lv1App theApp;
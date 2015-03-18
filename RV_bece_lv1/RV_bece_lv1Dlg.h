// RV_bece_lv1Dlg.h : header file
//
#include "highgui.h"
#include "cv.h"
#include "RV2DSIFT.h"
#include <vector>
#include <iostream>
#pragma once

// CRV_bece_lv1Dlg dialog
class CRV_bece_lv1Dlg : public CDialog
{
// Construction
public:
	CRV_bece_lv1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RV_BECE_LV1_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCanny();
	afx_msg void OnBnClickedCropImage();
	afx_msg void OnBnClickedVideo();
	afx_msg void OnBnClickedTemplateMatching();
	afx_msg void OnBnClickedHoughDetector();
	afx_msg void OnBnClickedRunSIFT();
	afx_msg void OnBnClickedFirstImage_SIFT();
	afx_msg void OnBnClickedSecondImage_SIFT();
	afx_msg void OnBnClickedRANSAC();
	afx_msg void OnBnDetectDominant();
};

// -------------- LV6 HEADER ----------------

struct RV3DPOINT
{
	int u, v, d;
};

void ReadKinectPics(CString pathRGB, IplImage *DepthImage, int *DepthMap, RV3DPOINT *Point3DArray, int &n3DPoints);
BOOL Plane(RV3DPOINT *pPoint3D0, RV3DPOINT *pPoint3D1, RV3DPOINT *pPoint3D2, double &a, double &b, double &c);
void Consensus(RV3DPOINT *Point3DArray, int n3DPoints, double &a, double &b, double &c, RV3DPOINT **ConsensusSet, int &nConsensus);

inline int RVRandom(int yMin, int yMax)
{
	return  (rand() % yMax) + yMin;
}

#include "stdafx.h"
#include "RV_bece_lv1.h"
#include "RV_bece_lv1Dlg.h"

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

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

CRV_bece_lv1Dlg::CRV_bece_lv1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRV_bece_lv1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRV_bece_lv1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRV_bece_lv1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CRV_bece_lv1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CRV_bece_lv1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRV_bece_lv1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CRV_bece_lv1Dlg::OnBnClickedButtonCanny)
	ON_BN_CLICKED(IDC_BUTTON3, &CRV_bece_lv1Dlg::OnBnClickedCropImage)
	ON_BN_CLICKED(IDC_BUTTON4, &CRV_bece_lv1Dlg::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_BUTTON5, &CRV_bece_lv1Dlg::OnBnClickedTemplateMatching)
	ON_BN_CLICKED(IDC_BUTTON6, &CRV_bece_lv1Dlg::OnBnClickedHoughDetector)
	ON_BN_CLICKED(IDC_BUTTON7, &CRV_bece_lv1Dlg::OnBnClickedRunSIFT)
	ON_BN_CLICKED(IDC_BUTTON8, &CRV_bece_lv1Dlg::OnBnClickedFirstImage_SIFT)
	ON_BN_CLICKED(IDC_BUTTON9, &CRV_bece_lv1Dlg::OnBnClickedSecondImage_SIFT)
	ON_BN_CLICKED(DETECT_DOMINANT, &CRV_bece_lv1Dlg::OnBnDetectDominant)
END_MESSAGE_MAP()

BOOL CRV_bece_lv1Dlg::OnInitDialog()
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


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRV_bece_lv1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRV_bece_lv1Dlg::OnPaint()
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
HCURSOR CRV_bece_lv1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------------
//---------------------------------- LV1 - UVOD --------------------------------------
//------------------------------------------------------------------------------------

// ------------------------------------------- GLOBALNE VARIJABLE --------------------------------------------
CString path=""; // put do ucitane slike
CvRect box = cvRect(-1, -1, 0, 0); // pocetna vrijednost definiranog pravokutnika (x,y,duljina,visina)
CvCapture *capture = cvCreateCameraCapture(0);
//CvCapture* capture = cvCaptureFromCAM(1); // 0 - manycam kamera, 1-logitech kamera
IplImage *pImageIn_tm=NULL; //Pokazivaè na sliku 
IplImage *image_edit=NULL; // pokazivac na sliku koja ce sluziti za obradu
IplImage *imageHoughShow= NULL;
IplImage *imageHoughLoop = NULL;
IplImage *frame=NULL;
int klikovi = 0; // brojac klikova
bool drawing_box = false; // bool varijabla ako se crta pravokutnik (oznacava dio slike)
bool template_matching= false; // bool za odabir template_matching moda rada u callbacku
bool hough=false, imageSel2Points=false;
Point imageClickPoint;
// -----------------------------------------------------------------------------------------------------------

// ----------------------------------------- PRE-DEFINIRANE FUNKCIJE -----------------------------------------
void my_mouse_callback(int event, int x, int y, int flags, void* param); // pre-definirani callback za mis
void draw_box(IplImage* img, CvRect rect); // pre-definirana funkcija za crtanje pravokutnika (okvira) odabira
void izdvoji_sliku(IplImage* img, CvRect rect); // pre-definirana funkcija za izdvajanje slike
void template_match(IplImage* img, CvRect rect); // pre-definirana funkcija za template match
// -----------------------------------------------------------------------------------------------------------

void loadImage()
{
	klikovi = 0; // Brojac klikova
	IplImage *pImageIn; //Pokazivaè na sliku 
	pImageIn = cvLoadImage(path); //Uèitavanje slike 
	if (template_matching) pImageIn_tm = cvLoadImage(path); //Uèitavanje slike  	
	cvNamedWindow("Original image"); //Stvaranje prozora u kojem æe slika biti prikazana 
	cvShowImage("Original image", pImageIn); //Prikaz slike 
	cvReleaseImage(&pImageIn);
}

void CRV_bece_lv1Dlg::OnBnClickedButton1() //IMAGE OPEN
{
	// Ucitavanje slike preko prozora za odabir slike
	CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	"Image files (*.bmp; *.jpg; *.pgm; *.png) |*.bmp;*.jpg;*.pgm;*.png |All Files (*.*)|*.*||", NULL);
	dlg.m_ofn.lpstrTitle = _T("Select Image");

	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName(); // Sadrži odabranu datotekeu
		loadImage();
	}
}

void CRV_bece_lv1Dlg::OnBnClickedOk()
{
	cvDestroyAllWindows();
	cvReleaseImage(&pImageIn_tm);
	cvReleaseImage(&image_edit);
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	OnCancel();
}

void CRV_bece_lv1Dlg::OnBnClickedCancel()
{
	cvDestroyAllWindows();
	cvReleaseImage(&pImageIn_tm);
	cvReleaseImage(&image_edit);
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	OnCancel();
}

//-------------------------------------------------------------------------------------
//----------------------------- LV1 - CANNYEV DETEKTOR --------------------------------
//-------------------------------------------------------------------------------------

void CRV_bece_lv1Dlg::OnBnClickedButtonCanny()
{
	if (path != ""){
	IplImage *pImageIn; //Pokazivaè na sliku 
	pImageIn = cvLoadImage(path); //Uèitavanje slike 
	IplImage *pImageOut = cvCreateImage(cvSize(pImageIn->width, pImageIn->height), IPL_DEPTH_8U, 1);
	cvCvtColor(pImageIn, pImageOut, CV_BGR2GRAY);
	cvCanny(pImageOut, pImageOut, 50, 200, 3);
	cvNamedWindow("Processed image");
	cvShowImage("Processed image", pImageOut);
	cvReleaseImage(&pImageOut);
	}
}
//-------------------------------------------------------------------------------------
//----------------------------- LV1 - IZDVAJANJE SLIKE --------------------------------
//-------------------------------------------------------------------------------------
void draw_box(IplImage* img, CvRect rect)
{
	cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
	cvScalar(0x00, 0x00, 0xff)); // blue, green, red
}

void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	IplImage* image = (IplImage*)param;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = cvRect(x, y, 0, 0);
		klikovi++;
		break;

	case CV_EVENT_MOUSEMOVE:
		if (drawing_box)
		{
			box.width = x - box.x; // pocetna - trenutna
			box.height = y - box.y;
		}
		break;

	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0)
		{
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0)
		{
			box.y += box.height;
			box.height *= -1;
		}
		if (template_matching && klikovi > 0)
		{
			image_edit = cvCloneImage(pImageIn_tm);
			template_match(image_edit, box);
		}
		else { if (klikovi > 0) izdvoji_sliku(image, box); }
		break;
	}
}

void izdvoji_sliku(IplImage* img, CvRect rect)
{
	cvSetImageROI(img, rect);
	IplImage *img2 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCopy(img, img2, NULL);
	cvResetImageROI(img);
	if (hough)
	{
		imageHoughShow = cvCloneImage(img2);
		imageHoughLoop = cvCloneImage(imageHoughShow);
	}
	else
	{ 
	cvNamedWindow("Image cut-out", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image cut-out", img2);
	cvReleaseImage(&img2);
	}
}

void CRV_bece_lv1Dlg::OnBnClickedCropImage()
{
	if (path != ""){
		loadImage();
		IplImage *pImageIn; //Pokazivaè na sliku 
		pImageIn = cvLoadImage(path); //Uèitavanje slike 
		cvSetMouseCallback("Original image", my_mouse_callback, (void*)pImageIn);
		IplImage* temp = cvCloneImage(pImageIn);
		while (1)
		{
			cvCopyImage(pImageIn, temp);
			if (drawing_box) draw_box(temp, box);
			cvShowImage("Original image", temp);
			if (cvWaitKey(15) == 27) break;
		}
		cvReleaseImage(&pImageIn);
		cvReleaseImage(&temp);
		cvDestroyWindow("Original image");
	}
}

//-------------------------------------------------------------------------------------
//----------------------------- LV1 - VIDEO STREAM/CAMERA -----------------------------
//-------------------------------------------------------------------------------------

void CRV_bece_lv1Dlg::OnBnClickedVideo()
{
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 30);
	assert(capture != NULL);
	cvNamedWindow("Camera capture", CV_WINDOW_AUTOSIZE);
	while ((frame = cvQueryFrame(capture)) != NULL)
	{
		cvShowImage("Camera capture", frame);
		if (cvWaitKey(33) == 27)
		{
			cvDestroyWindow("Camera capture");
			break;
		}
	}
}

//-------------------------------------------------------------------------------------
//----------------------------- LV2 - TEMPLATE MATCHING -------------------------------
//-------------------------------------------------------------------------------------

void CRV_bece_lv1Dlg::OnBnClickedTemplateMatching()
{
	if (path != ""){
		template_matching = true;
		loadImage();
		IplImage* image_show = cvCloneImage(pImageIn_tm);
		cvSetMouseCallback("Original image", my_mouse_callback, (void*)image_edit);
		while (1)
		{
			cvCopyImage(pImageIn_tm, image_show);
			if (drawing_box) draw_box(image_show, box);
			cvShowImage("Original image", image_show);
			if (cvWaitKey(15) == 27) break;
		}
		template_matching = false;
		cvReleaseImage(&pImageIn_tm);
		cvReleaseImage(&image_show);
		cvDestroyWindow("Original image");
	}
}

void template_match(IplImage* img, CvRect rect)
{
	cvSetImageROI(img, rect);
	IplImage *templ = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCopy(img, templ, NULL);
	cvResetImageROI(img);
	int iwidth = img->width - templ->width + 1;
	int iheight = img->height - templ->height + 1;

	// Allocate Output Image:
	IplImage *res = cvCreateImage(cvSize(iwidth, iheight), IPL_DEPTH_32F, 1); //32

	int istep = res->widthStep;

	/* perform template matching */
	cvMatchTemplate(img, templ, res, CV_TM_CCOEFF_NORMED);

	/* find location of best matches */
	float thresh = 0.8f;

	int windowSize = 11;  //must be an odd number

	int windowRad = (int)(windowSize / 2);

	float centerPixelValue, currentPixelValue;
	bool bcenterPixelMax = false;
	uchar *pImageData = (uchar *)res->imageData;

	for (int y = 0; y<iheight; y++)
	{
		for (int x = 0; x<iwidth; x++)
		{
			//subwindow
			if ((x - windowRad >= 0) &&
				(x + windowRad <= iwidth - 1) &&
				(y - windowRad >= 0) &&
				(y + windowRad <= iheight - 1)
				)
			{
				centerPixelValue = ((float *)(pImageData + istep * y))[x];

				if (centerPixelValue > thresh)
				{
					bcenterPixelMax = true;

					for (int j = y - windowRad; j <= y + windowRad; j++)
					{
						for (int i = x - windowRad; i <= x + windowRad; i++)
						{
							currentPixelValue = ((float *)(pImageData + istep * j))[i];

							if (currentPixelValue > centerPixelValue)
							{
								bcenterPixelMax = false;
								break; // exit i
							}
						}

						if (!bcenterPixelMax)
							break;  //exit j
					}
					if (bcenterPixelMax)  /* draw rectangle */
						cvRectangle(img,
						cvPoint(x, y),
						cvPoint(x + templ->width, y + templ->height),
						cvScalar(0x00, 0xff, 0x00));
				}
			}
		}
	}
	cvNamedWindow("Matched Template", CV_WINDOW_AUTOSIZE);
	cvShowImage("Matched Template", img);
	cvReleaseImage(&templ);
	cvReleaseImage(&res);
}

//-------------------------------------------------------------------------------------
//----------------------------- LV3 - HOUGHOV DETEKTOR -------------------------------
//-------------------------------------------------------------------------------------
int clicks = 0;

Point2f points_roi[4];
void my_mouse_callback_roi(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
	{
		Point pt(x, y);

			if (clicks < 4)
			{
				points_roi[clicks] = pt;
				clicks++;
				if (clicks == 4){ imageSel2Points = true; }
			}
	}break;
	    
	case CV_EVENT_MOUSEMOVE:
	{}break;

	case CV_EVENT_LBUTTONUP: 
	{}break;
	}
}



void CRV_bece_lv1Dlg::OnBnClickedHoughDetector()
{
	hough = true;

	//DEBUG CONSOLE
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");
	//END DEBUG

	clicks = 0;
	CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	CvMat *distortion = (CvMat*)cvLoad("Distortion.xml");
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 30);
	IplImage *image = NULL;

	// ----------- LOAD IMAGE FROM COMPUTER ---------------------

	CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		"Image files (*.bmp; *.jpg; *.pgm; *.png) |*.bmp;*.jpg;*.pgm;*.png |All Files (*.*)|*.*||", NULL);
	dlg.m_ofn.lpstrTitle = _T("Select Image");
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName(); // Sadrži odabranu datotekeu
		image = cvLoadImage(path); //Uèitavanje slike 
	}

	// ----------- CAPTURE FROM VIDEO --------------------------
	//image = cvQueryFrame(capture);

	IplImage *t = cvCloneImage(image);
	IplImage* mapx = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
	IplImage* mapy = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
	cvInitUndistortMap(intrinsic, distortion, mapx, mapy);
	cvRemap(t, image, mapx, mapy);
	//cvSaveImage("test.jpg", t);
	// -------------- DETERMINE ROI --------------------------//

	cvNamedWindow("Undistorted Image");
	cvSetMouseCallback("Undistorted Image", my_mouse_callback_roi, (void*)t);
	while (1)
	{
		cvCopyImage(image, t);
		cvShowImage("Undistorted Image", t);
		if (cvWaitKey(15) == 27 || imageSel2Points) break;

	}	
	Point roi_min, roi_max;
	roi_min.x = 9999; roi_min.y = 9999;
	roi_max.x = -1; roi_min.y = -1;

	for (int it = 0; it < 4; it++)
	{
		if (points_roi[it].x > roi_max.x) roi_max.x = points_roi[it].x;
		if (points_roi[it].y > roi_max.y) roi_max.y = points_roi[it].y;
		if (points_roi[it].x < roi_min.x) roi_min.x = points_roi[it].x;
		if (points_roi[it].y < roi_min.y) roi_min.y = points_roi[it].y;
	}

	CvRect roi_rect = cvRect(points_roi[0].x, points_roi[0].y, (roi_max.x - roi_min.x), (roi_max.y - roi_min.y));
	// -------------- DETERMINE ROI END--------------------------//

	izdvoji_sliku(image, roi_rect);
	IplImage *imageCanny = cvCreateImage(cvSize(imageHoughShow->width, imageHoughShow->height), IPL_DEPTH_8U, 1);
	cvCvtColor(imageHoughShow, imageCanny, CV_RGB2GRAY);
	cvCanny(imageCanny, imageCanny, 60, 200, 3);
	cvShowImage("Canny", imageCanny);
	CvSeq* lines;
	CvMemStorage* storage = cvCreateMemStorage(0);
	lines = cvHoughLines2(imageCanny, storage, CV_HOUGH_STANDARD, 1, CV_PI / 180, 80);
	Mat tocke = cvCreateMat(1, 4, CV_32FC2);
	float rho,theta = 0;
	double theta_cos,theta_sin=0;
	for (int i = 0; i < 1; i = i++) // 4 najdominantnija
	{
		float* line = (float*)cvGetSeqElem(lines, i);
		rho = line[0];
		theta = line[1];
		theta_cos = cos(theta); theta_sin = sin(theta);
		rho = rho + points_roi[0].x* theta_cos + points_roi[0].y* theta_sin;
		cout << "\n-------------------- IMAGE --------------------\n";
		cout << "\n\tTHETA: " << theta << "\n\tRHO:" << rho << "     "; // DEBUG

		CvPoint pt1, pt2;
		//double x0 = theta_cos*rho + point_roi_x, y0 = theta_sin*rho + point_roi_y;
		double x0 = theta_cos*rho, y0 = theta_sin*rho;
		pt1.x = cvRound(x0 + 1000 * (-theta_sin));
		pt1.y = cvRound(y0 + 1000 * (theta_cos));
		pt2.x = cvRound(x0 - 1000 * (-theta_sin));
		pt2.y = cvRound(y0 - 1000 * (theta_cos));
		cvLine(image, pt1, pt2, CV_RGB(255, 0, 0), 3, 8);
	}
	//rho = rho + sqrt(point_roi_x*point_roi_x + point_roi_y*point_roi_y);	

	hough = false;
	imshow("Hough", image);

	CvMat* imagePoints = (CvMat*)cvCreateMat(4, 2, CV_32FC1);
	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 0, 0)) = points_roi[0].x;
	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 0, 1)) = points_roi[0].y;

	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 1, 0)) = points_roi[1].x;
	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 1, 1)) = points_roi[1].y;

	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 2, 0)) = points_roi[2].x;
	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 2, 1)) = points_roi[2].y;

	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 3, 0)) = points_roi[3].x;
	*((float*)CV_MAT_ELEM_PTR(*imagePoints, 3, 1)) = points_roi[3].y;

	CvMat* objectPoints = (CvMat*)cvCreateMat(4, 3, CV_32FC1);
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 0, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 0, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 0, 2)) = 0;

	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 1, 0)) = 270;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 1, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 1, 2)) = 0;


	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 2, 0)) = 270;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 2, 1)) = 190;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 2, 2)) = 0;

	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 3, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 3, 1)) = 190;
	*((float*)CV_MAT_ELEM_PTR(*objectPoints, 3, 2)) = 0;

	CvMat* m_r = (CvMat*)cvCreateMat(3, 1, CV_32FC1);
	CvMat* m_t = (CvMat*)cvCreateMat(1, 3, CV_32FC1);
	CvMat* m_t_transpose = (CvMat*)cvCreateMat(3, 1, CV_32FC1);


	cvFindExtrinsicCameraParams2(objectPoints, imagePoints, intrinsic, distortion, m_r, m_t);
	CvMat* m_r_rodriguez = (CvMat*)cvCreateMat(3, 3, CV_32FC1);
	cvRodrigues2(m_r, m_r_rodriguez);
	cvTranspose(m_t,m_t_transpose);

	CvMat* m_A = (CvMat*)cvCreateMat(3, 3, CV_32FC1);
	cvMatMul(intrinsic, m_r_rodriguez, m_A);

	CvMat* m_B = (CvMat*)cvCreateMat(3, 1, CV_32FC1);
	cvMatMul(intrinsic, m_t_transpose, m_B);
	
	float lx = m_A->data.fl[0*3+0]*theta_cos+m_A->data.fl[1*3+0]*theta_sin-rho*m_A->data.fl[2*3+0];
	//cout << "\n Lx: " << lx;

	float ly = m_A->data.fl[0*3+1] * theta_cos + m_A->data.fl[1 * 3 + 1] * theta_sin - rho*m_A->data.fl[2 * 3 + 1];
	//cout << "\n Ly: " << ly;

	float lro = m_B->data.fl[2] * rho - m_B->data.fl[0] * theta_cos - m_B->data.fl[1] * theta_sin;
	//cout << "\n Lro: " << lro;

	float theta_new = (atan2(ly, lx))*(180/3.1415);
	float rho_real = lro / sqrt((lx*lx)+(ly*ly));
	cout << "\n\n-------------------- RESULTS --------------------\n";
	cout << "\n\t" << "THETA: " <<theta_new<<" degrees"<< endl;
	cout << "\tRHO: " << rho_real <<" mm"<< endl << endl;
	} 

	//-------------------------------------------------------------------------------------
	//------------------------------- LV4 - SIFT I RANSAC ---------------------------------
	//-------------------------------------------------------------------------------------
	
	IplImage *SIFT_image1 = NULL; //Pokazivaè na sliku 
	IplImage *SIFT_image2 = NULL; // pokazivac na sliku koja ce sluziti za obradu
	IplImage *SIFT_image3;
	

	
int MatchSIFTDescriptors(RV2DSIFT_DESCRIPTOR *pModelDescriptor,RV2DSIFT_DESCRIPTOR *pSceneDescriptor)
	{

		int *pMData;
		int *pSData;
		int *pMDataEnd;

		int e;

		pMData = pModelDescriptor->Data;

		pSData = pSceneDescriptor->Data;

		pMDataEnd = pMData + 128;  //128 descriptorLength

		int E = 0;

		for (; pMData < pMDataEnd; pMData++, pSData++)
		{
			e = (int)(*pSData) - (int)(*pMData);

			if (e >= 0) E += e;
			else E -= e;
		}

		return E;
	}

void combineImages()
{
	cvSetImageROI(SIFT_image3, cvRect(0, 0, SIFT_image1->width, SIFT_image1->height));
	cvCopy(SIFT_image1, SIFT_image3, NULL);
	cvResetImageROI(SIFT_image3);

	cvSetImageROI(SIFT_image3, cvRect(SIFT_image2->width, 0, SIFT_image2->width, SIFT_image1->height));
	cvCopy(SIFT_image2, SIFT_image3, NULL);
	cvResetImageROI(SIFT_image3);

	cvShowImage("SIFT Images", SIFT_image3);
}

Point SIFT_ROI_start;
CvRect SIFT_BOX = cvRect(-1, -1, 0, 0);
bool siftBox = false;

// ----------------------------------------- SIFT ROI CALLBACK ------------------------------------------------- 

void SIFT_mouse_callback(int event, int x, int y, int flags, void* param)
{
	IplImage* image = (IplImage*)param;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		SIFT_BOX = cvRect(x, y, 0, 0);
		klikovi++;
		break;

	case CV_EVENT_MOUSEMOVE:
		if (drawing_box)
		{
			SIFT_BOX.width = x - SIFT_BOX.x;
			SIFT_BOX.height = y - SIFT_BOX.y;
		}
		break;

	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		siftBox = true;
		if (SIFT_BOX.width < 0)
		{
			SIFT_BOX.x += SIFT_BOX.width;
			SIFT_BOX.width *= -1;
		}
		if (SIFT_BOX.height < 0)
		{
			SIFT_BOX.y += SIFT_BOX.height;
			SIFT_BOX.height *= -1;
		}
		break;
	}
}

	void CRV_bece_lv1Dlg::OnBnClickedRunSIFT()
	{
		SIFT_BOX = cvRect(-1, -1, 0, 0);
		siftBox = false;
		drawing_box = false;
		IplImage* image=cvCloneImage(SIFT_image3);

		// ----------------------------------------- SET ROI ------------------------------------------------- 
		cvShowImage("SIFT Images", image);
		cvSetMouseCallback("SIFT Images", SIFT_mouse_callback, (void*)image);
		while (1)
		{
			cvCopyImage(SIFT_image3,image);
			if (drawing_box) draw_box(image, SIFT_BOX);
			cvShowImage("SIFT Images", image);
			if (cvWaitKey(15) == 27 || siftBox) break;

		}

		// ----------------------------------------- SIFT ------------------------------------------------- 
		CRV2DSIFT SIFT_model;
		CRV2DSIFT SIFT_scene;
		cvSetImageROI(SIFT_image1, SIFT_BOX);
		SIFT_model.CreateSIFT(SIFT_image1);
		cvResetImageROI(SIFT_image1);

		/**************************************/

		/***********************Get and mark SIFT features**************************/
		CvSeq* descriptors = SIFT_model.Descriptors;
		for (int i = 0; i < SIFT_model.m_nDescriptors; i++)
		{
			RV2DSIFT_DESCRIPTOR* desc = (RV2DSIFT_DESCRIPTOR*)cvGetSeqElem(descriptors, i);
			cvCircle(SIFT_image1, cvPoint(cvRound(desc->fu + SIFT_BOX.x), cvRound(desc->fv + SIFT_BOX.y)), 3, CV_RGB(255, 0, 0), -1, 8, 0);
		}

		SIFT_scene.CreateSIFT(SIFT_image2);
		CvSeq* descriptors_scene = SIFT_scene.Descriptors;

		for (int j = 0; j < SIFT_scene.m_nDescriptors; j++)
		{
			RV2DSIFT_DESCRIPTOR* descriptor = (RV2DSIFT_DESCRIPTOR*)cvGetSeqElem(descriptors_scene, j);
			cvCircle(SIFT_image2, cvPoint(cvRound(descriptor->fu), cvRound(descriptor->fv)), 3, CV_RGB(255, 0, 0),-1,8,0);
		}

		combineImages();

		int nM = SIFT_model.m_nDescriptors;	//number of descriptors in model
		int nS = SIFT_scene.m_nDescriptors;	//number of descriptors in scene

		CvSeq *pSiftModelDescriptors = SIFT_model.Descriptors;
		CvSeq *pSiftSceneDescriptors = SIFT_scene.Descriptors;

		int nReliableMatches = 0;

		MSMATCH_DATA *MatchM = new MSMATCH_DATA[nM];

		memset(MatchM, 0x7f, nM * sizeof(MSMATCH_DATA));

		MSMATCH_DATA *MatchS = new MSMATCH_DATA[nS];

		memset(MatchS, 0x7f, nS * sizeof(MSMATCH_DATA));


		MSMATCH_DATA *pMatchS = MatchS;
		MSMATCH_DATA *pMatchM;

		RV2DSIFT_DESCRIPTOR* pSObject;
		RV2DSIFT_DESCRIPTOR* pMObject;

		int e, iS, iM;

		for (iS = 0; iS < nS; iS++, pMatchS++)
		{
			pSObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftSceneDescriptors, iS);

			//reset relation
			pMatchS->bRelation = false;

			//reset model
			pMatchM = MatchM;

			for (iM = 0; iM < nM; iM++, pMatchM++)
			{
				//reset relation
				pMatchM->bRelation = false;

				pMObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftModelDescriptors, iM);

				e = MatchSIFTDescriptors(pMObject, pSObject);

				if (e < pMatchS->eBestMatch)
				{
					pMatchS->eBestMatch = e;
					pMatchS->iBestMatch = iM;
				}

				if (e < pMatchM->eBestMatch)
				{
					pMatchM->eBestMatch = e;
					pMatchM->iBestMatch = iS;
				}
			}	// for all MObjects
		}	// for all SObjects
		
		pMatchS = MatchS;
		combineImages();

		for (iS = 0; iS < nS; iS++, pMatchS++)
		{
			iM = pMatchS->iBestMatch;
			pMatchM = MatchM + iM;
			if (iS == pMatchM->iBestMatch && pMatchS->eBestMatch == pMatchM->eBestMatch)
			{
				//Set relation
				pMatchS->bRelation = true;
				pMatchM->bRelation = true;
				nReliableMatches++;

				pMObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftModelDescriptors, iM);
				pSObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftSceneDescriptors, iS);
				cvDrawLine(SIFT_image3, cvPoint(cvRound(pMObject->fu + SIFT_BOX.x), cvRound(pMObject->fv + SIFT_BOX.y)), cvPoint(cvRound(pSObject->fu + 640), cvRound(pSObject->fv)), CV_RGB(0, 255, 0), 1, 8, 0);
			}
		}
		cvShowImage("SIFT Images", SIFT_image3);


		// ----------------------------------------- RANSAC ------------------------------------------------- 

		CvMat* points1, *points2, *status, *fundamentalMatrix;
		points1 = cvCreateMat(2, nReliableMatches, CV_32FC1);
		points2 = cvCreateMat(2, nReliableMatches, CV_32FC1);
		status = cvCreateMat(1, nReliableMatches, CV_8UC1);

		int iterations = 0;
		pMatchS = MatchS;
		pMatchM = MatchM;

		for (iS = 0; iS<nS; iS++, pMatchS++)
		{
			iM = pMatchS->iBestMatch;
			pMatchM = MatchM + iM;
			if (iS == pMatchM->iBestMatch && pMatchS->eBestMatch == pMatchM->eBestMatch)
			{

				pMObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftModelDescriptors, iM);
				pSObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftSceneDescriptors, iS);

				cvSetReal2D(points1, 0, iterations, pMObject->fu);
				cvSetReal2D(points1, 1, iterations, pMObject->fv);

				cvSetReal2D(points2, 0, iterations, pSObject->fu);
				cvSetReal2D(points2, 1, iterations, pSObject->fv);

				iterations++;
			}
		}

		fundamentalMatrix = cvCreateMat(3, 3, CV_32FC1);
		int fm_count = cvFindFundamentalMat(points1, points2, fundamentalMatrix, CV_FM_RANSAC, 3, 0.99, status);


		iterations = 0;
		int r = 0;
		pMatchS = MatchS;
		pMatchM = MatchM;
		CvPoint pt_min, pt_max;
		pt_min.x = 9999;
		pt_min.y = 9999;
		pt_max.x = 0;
		pt_max.y = 0;

		for (iS = 0; iS<nS; iS++,pMatchS++)
		{
			iM = pMatchS->iBestMatch;
			pMatchM = MatchM + iM;
			if (iS == pMatchM->iBestMatch && pMatchS->eBestMatch == pMatchM->eBestMatch)
			{
				if (status->data.ptr[iterations] == 1)
				{
					iM = pMatchS->iBestMatch;

					pMObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftModelDescriptors, iM);
					pSObject = (RV2DSIFT_DESCRIPTOR *)cvGetSeqElem(pSiftSceneDescriptors, iS);

					if (pt_min.x>cvRound(pSObject->fu) + 640 && pt_min.y > cvRound(pSObject->fv))
					{
						pt_min.x = pSObject->fu + 640;
						pt_min.y = pSObject->fv;
					}


					if (pt_max.x<cvRound(pSObject->fu)+640 && pt_max.y < cvRound(pSObject->fv))
					{
						pt_max.x = pSObject->fu + 640;
						pt_max.y = pSObject->fv;
					}

					if (pt_max.x>cvRound(pSObject->fu) + 640 && pt_max.y < cvRound(pSObject->fv))
					{
						pt_max.y = pSObject->fv;
					}

					if (pt_max.x<cvRound(pSObject->fu) + 640 && pt_max.y > cvRound(pSObject->fv))
					{
						pt_max.x = pSObject->fu + 640;
					}


					cvCircle(image, cvPoint(cvRound(pMObject->fu + SIFT_BOX.x), cvRound(pMObject->fv + SIFT_BOX.y)), 3, CV_RGB(255, 0, 0), -1, 8, 0);
					cvCircle(image, cvPoint(cvRound(pSObject->fu + 640), cvRound(pSObject->fv)), 3, CV_RGB(255, 0, 0), -1, 8, 0);

					cvDrawLine(image, cvPoint(cvRound(pMObject->fu + SIFT_BOX.x), cvRound(pMObject->fv + SIFT_BOX.y)), cvPoint(cvRound(pSObject->fu + 640), cvRound(pSObject->fv)), CV_RGB(0, 255, 0), 1, 8, 0);
				}

				iterations++;
			}
		}
		cvDrawRect(image, pt_min, pt_max, CV_RGB(0, 0, 255),2,8,0);
		cvNamedWindow("RANSAC images");
		cvShowImage("RANSAC images", image);
	}


	void CRV_bece_lv1Dlg::OnBnClickedFirstImage_SIFT()
	{

		CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			"Image files (*.bmp; *.jpg; *.pgm; *.png) |*.bmp;*.jpg;*.pgm;*.png |All Files (*.*)|*.*||", NULL);
		dlg.m_ofn.lpstrTitle = _T("Select Image");

		if (dlg.DoModal() == IDOK)
		{
			path = dlg.GetPathName();
			SIFT_image1 = cvLoadImage(path); 
			cvNamedWindow("SIFT Images");
			cvShowImage("SIFT Images", SIFT_image1); 
		}

	}

	void CRV_bece_lv1Dlg::OnBnClickedSecondImage_SIFT()
	{
		CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			"Image files (*.bmp; *.jpg; *.pgm; *.png) |*.bmp;*.jpg;*.pgm;*.png |All Files (*.*)|*.*||", NULL);
		dlg.m_ofn.lpstrTitle = _T("Select Image");

		if (dlg.DoModal() == IDOK)
		{
			path = dlg.GetPathName(); 
			SIFT_image2 = cvLoadImage(path);
			int dstWidth = SIFT_image1->width + SIFT_image1->width;
			int dstHeight = SIFT_image1->height;
			SIFT_image3 = cvCreateImage(cvSize(dstWidth, dstHeight), IPL_DEPTH_8U, 3);

			combineImages();

		}
	}


	void CRV_bece_lv1Dlg::OnBnClickedRANSAC()
	{
		// TODO: Add your control notification handler code here
	}

	//-------------------------------------------------------------------------------------
	//---------------------- LV6 - ODREDJIVANJE DOMINANTNE RAVNINE ------------------------
	//-------------------------------------------------------------------------------------

	void ReadKinectPics(CString pathRGB, IplImage *DepthImage, int *DepthMap, RV3DPOINT *Point3DArray, int &n3DPoints)
	{

		int u, v, d;
		int dmin = 2047;
		int dmax = 0;

		RV3DPOINT *pPoint3D = Point3DArray;

		n3DPoints = 0;

		//Get DepthImage file path
		CString pathDepth = pathRGB.Left(pathRGB.GetLength() - 4);
		pathDepth = pathDepth + "-D.txt";

		FILE *fp;

		fopen_s(&fp, pathDepth, "r");

		if (fp)
		{
			BOOL bOK = TRUE;

			//Determine max and min depth values and get Depth map
			for (v = 0; v<DepthImage->height; v++)
			{
				for (u = 0; u<DepthImage->width; u++)
				{
					if (!(bOK = (fscanf(fp, "%d ", &d) == 1)))
						break;


					if (d == 2047)
					{
						d = -1;
					}
					else
					{
						//count number of valid 3D points
						n3DPoints++;

						//determine min and max d
						if (d<dmin)
							dmin = d;

						if (d>dmax)
							dmax = d;
					}

					DepthMap[v*DepthImage->width + u] = d;


					if (d != -1)
					{
						pPoint3D->u = u;
						pPoint3D->v = v;
						pPoint3D->d = d;

						pPoint3D++;
					}


				}
			}

			fclose(fp);
		}

		//Form grayscale pic -> Scale from 1 to 255 (reserve 0 for undefined regions)
		for (v = 0; v<DepthImage->height; v++)
		{
			for (u = 0; u<DepthImage->width; u++)
			{
				d = DepthMap[v*DepthImage->width + u];

				if (d != -1)
					d = ((d - dmin) * 254 / (dmax - dmin)) + 1;
				else
					d = 0;

				((uchar *)(DepthImage->imageData + v*DepthImage->widthStep))[u] = d;
			}
		}
	}


	BOOL Plane(RV3DPOINT *pPoint3D0, RV3DPOINT *pPoint3D1, RV3DPOINT *pPoint3D2, double &a, double &b, double &c)
	{
		double A[3 * 3];

		A[0 * 3 + 0] = pPoint3D0->u;
		A[0 * 3 + 1] = pPoint3D0->v;
		A[0 * 3 + 2] = 1.0;

		A[1 * 3 + 0] = pPoint3D1->u;
		A[1 * 3 + 1] = pPoint3D1->v;
		A[1 * 3 + 2] = 1.0;

		A[2 * 3 + 0] = pPoint3D2->u;
		A[2 * 3 + 1] = pPoint3D2->v;
		A[2 * 3 + 2] = 1.0;

		double Z[3];

		Z[0] = pPoint3D0->d;
		Z[1] = pPoint3D1->d;
		Z[2] = pPoint3D2->d;

		CvMat *A_ = cvCreateMatHeader(3, 3, CV_64FC1);
		A_->data.db = A;
		CvMat *Z_ = cvCreateMatHeader(3, 1, CV_64FC1);
		Z_->data.db = Z;

		double p[3];
		CvMat *p_ = cvCreateMatHeader(3, 1, CV_64FC1);
		p_->data.db = p;

		if (cvSolve(A_, Z_, p_))
		{
			a = p[0];
			b = p[1];
			c = p[2];

			cvReleaseMatHeader(&A_);
			cvReleaseMatHeader(&Z_);
			cvReleaseMatHeader(&p_);

			return TRUE;
		}
		else
		{
			cvReleaseMatHeader(&A_);
			cvReleaseMatHeader(&Z_);
			cvReleaseMatHeader(&p_);

			return FALSE;
		}
	}


	void Consensus(RV3DPOINT *Point3DArray, int n3DPoints, double &a, double &b, double &c, RV3DPOINT **ConsensusSet, int &nConsensus)
	{
		RV3DPOINT *pPoint3D = Point3DArray;

		RV3DPOINT **pConsensusSet = ConsensusSet;
		double Tol = 4.0;

		int i;
		double e;

		nConsensus = 0;

		for (i = 0; i < n3DPoints; i++, pPoint3D++)
		{
			e = pPoint3D->d - (a * pPoint3D->u + b * pPoint3D->v + c);

			if (e > Tol)
				continue;

			if (e >= -Tol)
				*(pConsensusSet++) = pPoint3D;


		}

		nConsensus = pConsensusSet - ConsensusSet;

	}



	void CRV_bece_lv1Dlg::OnBnDetectDominant()
	{
		IplImage *pRGBImage = 0;
		IplImage *pDepthImage = 0;
		int *DepthMap = 0;
		int n3DPoints = 0;

		int iWidth, iHeight;

		RV3DPOINT *Point3DArray = 0;

		CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			"Image files (*.bmp; *.jpg; *.pgm; *.png) |*.bmp;*.jpg;*.pgm;*.png|All Files (*.*)|*.*||", NULL);

		dlg.m_ofn.lpstrTitle = _T("Select Image");

		if (dlg.DoModal() == IDOK)
		{
			CString pathRGB = dlg.GetPathName();

			//Get RGB image
			pRGBImage = cvLoadImage(pathRGB);

			iWidth = pRGBImage->width;
			iHeight = pRGBImage->height;

			DepthMap = new int[iWidth * iHeight];
			memset(DepthMap, 0, iWidth * iHeight*sizeof(int));

			Point3DArray = new RV3DPOINT[iWidth * iHeight];

			memset(Point3DArray, 0, iWidth * iHeight * sizeof(RV3DPOINT));

			pDepthImage = cvCreateImage(cvSize(pRGBImage->width, pRGBImage->height), IPL_DEPTH_8U, 1);

			ReadKinectPics(pathRGB, pDepthImage, DepthMap, Point3DArray, n3DPoints);

			cvNamedWindow("RGB image");
			cvShowImage("RGB image", pRGBImage);

			cvNamedWindow("Depth image");
			cvShowImage("Depth image", pDepthImage);
		}

		double a, b, c;
		double aBest, bBest, cBest;

		int nConsensus;
		int nBest = 0;
		int iter;

		int maxRANSACIter = 1000; //Max RANSAC iterations

		RV3DPOINT *pPoint3D, *pPoint3D0, *pPoint3D1, *pPoint3D2;

		RV3DPOINT **Point3DPtrBuff1 = new RV3DPOINT *[n3DPoints];
		RV3DPOINT **Point3DPtrBuff2 = new RV3DPOINT *[n3DPoints];

		RV3DPOINT **ConsensusSet = Point3DPtrBuff1;
		RV3DPOINT **BestConsensusSet = Point3DPtrBuff2;

		RV3DPOINT **tmp;

		int iR0, iR1, iR2;

		//main RANSAC loop
		for (iter = 0; iter<maxRANSACIter; iter++)
		{
			// randomly select 3 points from set A
			iR0 = RVRandom(0, n3DPoints - 1);
			iR1 = RVRandom(0, n3DPoints - 1);
			iR2 = RVRandom(0, n3DPoints - 1);

			pPoint3D0 = (Point3DArray + iR0);
			pPoint3D1 = (Point3DArray + iR1);
			pPoint3D2 = (Point3DArray + iR2);

			if (!Plane(pPoint3D0, pPoint3D1, pPoint3D2, a, b, c))
				continue;

			Consensus(Point3DArray, n3DPoints, a, b, c, ConsensusSet, nConsensus);

			//Test best values and consensus set
			if (nConsensus > nBest)
			{
				nBest = nConsensus;

				aBest = a;
				bBest = b;
				cBest = c;

				tmp = ConsensusSet;
				ConsensusSet = BestConsensusSet;
				BestConsensusSet = tmp;
			}

		}

		IplImage *pColor = cvCreateImage(cvGetSize(pDepthImage), IPL_DEPTH_8U, 3);
		cvCvtColor(pDepthImage, pColor, CV_GRAY2RGB);


		for (int i = 0; i<nBest; i++)
		{
			pPoint3D = BestConsensusSet[i];
			((uchar *)(pColor->imageData + pPoint3D->v*pColor->widthStep))[pPoint3D->u * 3] = 0;
			((uchar *)(pColor->imageData + pPoint3D->v*pColor->widthStep))[pPoint3D->u * 3 + 1] = 255;
			((uchar *)(pColor->imageData + pPoint3D->v*pColor->widthStep))[pPoint3D->u * 3 + 2] = 0;

		}

		cvNamedWindow("Dominant plane");
		cvShowImage("Dominant plane", pColor);

		//exit elegantly 
		delete[] DepthMap;
		delete[] Point3DArray;
		delete[] Point3DPtrBuff1;
		delete[] Point3DPtrBuff2;
	}

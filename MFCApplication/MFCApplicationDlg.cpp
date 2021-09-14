
// MFCApplicationDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CStatic m_PictureCtrl;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CAboutDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CMFCApplicationDlg dialog



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//ramya
	//DDX_Control(pDX, IDC_STATIC, m_PictureCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplicationDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCApplicationDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplicationDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplicationDlg message handlers

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

void CMFCApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	vector<uchar> *stream = (vector<uchar>*)userdata;
	size_t count = size * nmemb;
	stream->insert(stream->end(), ptr, ptr + count);
	return count;
}

//function to retrieve the image as cv::Mat data type
cv::Mat curlImg(const char *img_url, int timeout = 10)
{
	vector<uchar> stream;
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
	CURLcode res = curl_easy_perform(curl); // start curl
	curl_easy_cleanup(curl); // cleanup
	return imdecode(stream, -1); // 'keep-as-is'
}
void GetIdResposne(std::string id);
void writeCSV()
{
	for (auto id : IdList)
	{
		GetIdResposne(id);
	}
	fstream fout;

	// opens an existing csv file or creates a new file.
	fout.open("userdata.csv", ios::out | ios::app);

	

	//int i, roll, phy, chem, math, bio;
	string name;

	// Read the input
	for (auto itr : userData)
	{
		auto va = itr.second;
		// Insert the data to file
		fout << itr.first << ", "
			<< va.fName << ", "
			<< va.lName << ", "
			<< va.genter << ", "
			<< va.dob << ", "
			<< va.email << ", "
			<< va.mbl << ", "
			<< va.regDate << ", "
			<< va.dob << ", "
			<< va.loc.city << ", "
			<< va.loc.country << ", "
			<< va.loc.state << ", "
			<< va.loc.street << ", "
			<< va.loc.timezone 
			<< "\n";
	}

}


void BuildApiData()
{

	std::string apiResponse = GetApiData("https://dummyapi.io/data/v1/user?limit=50");
	Document document;
	document.Parse(apiResponse.c_str());
	if (document.IsObject())
	{

		for (auto& data : document.GetObj())
		{
			if (data.name == "data")
			{
				for (auto& itr : data.value.GetArray())
				{
					//if (itr.IsObject())
					for (auto& info : itr.GetObj())
					{
						std::string key = info.name.GetString();
						auto &val = info.value;
						std::string strVal;
						if (key == "id")
						{
							IdList.push_back(val.GetString());
							break;
						}
					}
				}
			}
		}

	}	

	//std::thread t(writeCSV);
}

void GetIdResposne(std::string id)
{

	//for(auto id: IdList)
	static std::mutex io_mutex;
	{
		std::string apiRes = GetApiData("https://dummyapi.io/data/v1/user/" + id);
		if (!apiRes.empty())
		{
			Document doc;
			doc.Parse(apiRes.c_str());
			if (doc.IsObject())
			{
				attributes attribute;
				string id;
				for (auto& attr : doc.GetObj())
				{
					std::string key = attr.name.GetString();
					auto &val = attr.value;
					if (val.IsString())
					{
						std::string strVal = val.GetString();
						if (key == "id")
							id = strVal;
						else if (key == "title")
							attribute.title = strVal;
						else if (key == "firstName")
							attribute.fName = strVal;
						else if (key == "lastName")
							attribute.lName = strVal;
						else if (key == "picture")
							attribute.pic = strVal;
						else if (key == "gender")
							attribute.genter = strVal;
						else if (key == "email")
							attribute.email = strVal;
						else if (key == "dateOfBirth")
							attribute.dob = strVal;
						else if (key == "phone")
							attribute.mbl = strVal;
						else if (key == "registerDate")
							attribute.regDate = strVal;
						else if (key == "updatedDate")
							attribute.updateDate = strVal;
					}
					else if (val.IsObject() && key == "location")
					{
						for (auto& loc : val.GetObj())
						{
							std::string key = loc.name.GetString();
							auto &loc_val = loc.value;
							if (loc_val.IsString())
							{
								std::string str = loc_val.GetString();
								if (key == "street")
									attribute.loc.street = str;
								else if (key == "city")
									attribute.loc.city = str;
								else if (key == "state")
									attribute.loc.state = str;
								else if (key == "country")
									attribute.loc.country = str;
								else if (key == "timezone")
									attribute.loc.timezone = str;

							}
						}
					}
				}
				{
					std::lock_guard<std::mutex> lk(io_mutex);
					userData.insert({ id,attribute });
				}
			}
		}
		

	}
}

void CMFCApplicationDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}

	/*MessageBox(L"hgd");*/


	comboBoxCtrl = (CComboBox *)GetDlgItem(IDC_COMBO1);

	BuildApiData();

	CString text;
	//text = CString(userData.begin()->first.c_str());
	//
	CString str;
	int i = 1;
	comboBoxCtrl->AddString(CString("Select Id"));
	for (auto itr : IdList)
	{

		str.Format(CString(itr.c_str()), i);
		comboBoxCtrl->AddString(str);
		i++;
	}
	comboBoxCtrl->SetCurSel(comboBoxCtrl->FindStringExact(0, L"Select Id"));
}

HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	
	str = "hi";
	/*if (!str.IsEmpty())
		m_strTextCtrl = str;
	else
		m_strTextCtrl = _T("Write Something");*/
	//GetDlgItemText(IDC_EDIT2, str);
	GetDlgItemText(IDC_STATIC, str);
	//MessageBox(str);


}


void CAboutDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMFCApplicationDlg::OnBnClickedOk()
{

	// Data member in your dialog class 
	CStatic m_MyStaticControl;
	

		// Set its text (string loaded from string table) 
		CString text;
	//	text = "hdgfjfds";
	
		SetDlgItemText(IDC_STATIC, text);
}


void CMFCApplicationDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here


}

void ImgeShow(const std::string &id)
{

	//GetIdResposne(id);
	auto ita = userData.find(id);
	if (ita == userData.end())
	{
		GetIdResposne(id);
	}
	using namespace cv;
	
	Mat img(512, 1024, CV_8UC3, Scalar(234, 195, 184));
	auto it = userData.find(id);
	if (it != userData.end())
	{
		std::string filepath = "result/" + it->first + ".jpg";
		auto val = it->second;
		std::string pic = it->second.pic;
		//cv::putText(img, "Differencing the two images.", Point(30, 30),	1, 0.8, Scalar(200, 200, 250));
		cv::putText(img, "Name :" + val.fName+" "+val.lName, cv::Point(20, 30), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(200, 10,10), 1);
		cv::putText(img, "ID   :" + it->first, cv::Point(20, 70), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(200, 10, 10), 1);


		cv::putText(img, "Gender :"+ val.genter, cv::Point(10, 130), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Date of Birth :"+ val.dob, cv::Point(10, 160), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Register Date :"+ val.regDate, cv::Point(10, 190), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Email :"+ val.email, cv::Point(10, 220), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Phone :"+ val.mbl, cv::Point(10, 250), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Address :", cv::Point(10, 330), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(200, 100, 50), 1);
		cv::putText(img, "State   :" + val.loc.state, cv::Point(10, 360), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Street  :" + val.loc.street, cv::Point(10, 390), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "City    :" + val.loc.city, cv::Point(10, 420), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Country :" + val.loc.country, cv::Point(10, 450), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
		cv::putText(img, "Timezone:" + val.loc.timezone, cv::Point(10, 480), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);


		
		imwrite(filepath, img);
		Mat poto = curlImg(val.pic.c_str());
		Size size(200, 180);//the dst image size,e.g.100x100
		Mat dst;//dst image
		
		resize(poto, dst, size);
	

		Mat insetImage(img, Rect(800, 250, 200, 180));
		dst.copyTo(insetImage);
		imshow("ID:" + it->first, img);
		
		if (img.empty())
		{
			//std::cout << "Could not read the image: " << image_path << std::endl;
			//return 1;
		}
		int k = waitKey(0); // Wait for a keystroke in the window
		if (k == 's')
		{
			imwrite(it->first + ".jpg", img);
		}
	}
}


void CMFCApplicationDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here


	CString sData;
	int nSel = comboBoxCtrl->GetCurSel();
	if (nSel != LB_ERR)
	{
		comboBoxCtrl->GetLBText(nSel, sData);
		
	}

	if (sData == "Select Id")
		MessageBox(CString("Please Select Valid ID"));
	//else
	//	MessageBox(sData + CString(" Data Will be show another window"));
	std::wstring des(sData.GetString());
	std::string str(des.begin(), des.end());
	ImgeShow(str);
}

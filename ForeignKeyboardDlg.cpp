
// ForeignKeyboardDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ForeignKeyboard.h"
#include "ForeignKeyboardDlg.h"
#include "afxdialogex.h"
#include <htmlhelp.h>
#pragma comment(lib, "htmlhelp.lib")

namespace
{
	constexpr UINT SPECIAL_BUTTON_IDS[] = {
		IDC_BUTTON_COPY,
		IDC_BUTTON_CLEAR,
		IDC_BUTTON_SPACE,
		IDC_BUTTON_PERIOD,
		IDC_BUTTON_COMMA,
		IDC_BUTTON_SEMICOLON,
		IDC_BUTTON_COLON,
		IDC_BUTTON_QUOTATION,
		IDC_BUTTON_SINGLE_QUOTATION,
		IDC_BUTTON_DASH,
		IDC_BUTTON_UNDERBAR,
		IDC_BUTTON_ACUTE_ACCENT,
		IDC_BUTTON_GRAVE_ACCENT,
		IDC_BUTTON_CIRCUMFLEX,
		IDC_BUTTON_TREMA,
		IDC_BUTTON_TILDE,
		IDC_BUTTON_ASTERISK,
		IDC_BUTTON_EXCLAMATION,
		IDC_BUTTON_QUESTION,
		IDC_BUTTON_BACKSPACE,
		IDC_BUTTON_DEL,
		IDC_BUTTON_OCTOTHORPE,
		IDC_BUTTON_CURRENCY_SIGN,
		IDC_BUTTON_PERCENT,
		IDC_BUTTON_AMPERSAND,
		IDC_BUTTON_SOLIDUS,
		IDC_BUTTON_LEFT_PARENTHESIS,
		IDC_BUTTON_RIGHT_PARENTHESIS,
		IDC_BUTTON_EQUAL_SIGN,
		IDC_BUTTON_PLUS_SIGN,
		IDC_BUTTON_REVERSE_SOLIDUS,
		IDC_BUTTON_AMPERSAT,
		IDC_BUTTON_POUND_SIGN,
		IDC_BUTTON_DOLLAR_SIGN,
		IDC_BUTTON_EURO_SIGN,
		IDC_BUTTON_LEFT_BRACE,
		IDC_BUTTON_RIGHT_BRACE,
		IDC_BUTTON_LEFT_BRACKET,
		IDC_BUTTON_RIGHT_BRACKET,
		IDC_BUTTON_CENT_SIGN,
		IDC_BUTTON_YUAN_SIGN,
		IDC_BUTTON_HOME_KEY,
		IDC_BUTTON_END_KEY,
		IDC_BUTTON_LESS_THAN,
		IDC_BUTTON_GREATER_THAN,
		IDC_BUTTON_PIPE,
		IDC_BUTTON_NUMBER_ZERO,
		IDC_BUTTON_NUMBER_ONE,
		IDC_BUTTON_NUMBER_TWO,
		IDC_BUTTON_NUMBER_THREE,
		IDC_BUTTON_NUMBER_FOUR,
		IDC_BUTTON_NUMBER_FIVE,
		IDC_BUTTON_NUMBER_SIX,
		IDC_BUTTON_NUMBER_SEVEN,
		IDC_BUTTON_NUMBER_EIGHT,
		IDC_BUTTON_NUMBER_NINE,
		IDC_BUTTON_ONEHALF
	};
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	afx_msg void OnBnClickedButtonAbout();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CForeignKeyboardDlg dialog



CForeignKeyboardDlg::CForeignKeyboardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOREIGNKEYBOARD_DIALOG, pParent)
	, m_currentAlphabetIndex(-1)
	, m_shiftOn(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CForeignKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
	DDX_Control(pDX, IDC_BUTTON_SHIFT, m_btnShift);
}

BEGIN_MESSAGE_MAP(CForeignKeyboardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CForeignKeyboardDlg::OnCbnSelchangeComboLanguage)
	ON_BN_CLICKED(IDC_BUTTON_SHIFT, &CForeignKeyboardDlg::OnBnClickedButtonShift)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CForeignKeyboardDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CForeignKeyboardDlg::OnBnClickedButtonHelp)
	ON_COMMAND_RANGE(KEY_BUTTON_BASE, KEY_BUTTON_MAX, &CForeignKeyboardDlg::OnKeyboardButton)
	ON_COMMAND_RANGE(IDC_BUTTON_COPY, IDC_BUTTON_AMPERSAND, &CForeignKeyboardDlg::OnSpecialButton)
END_MESSAGE_MAP()


// CForeignKeyboardDlg message handlers

BOOL CForeignKeyboardDlg::OnInitDialog()
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

	InitAlphabets();
	InitLanguageCombo();
	InitSpecialButtonToolTips();

	if (!m_alphabets.empty())
	{
		for (int i = 0; i < m_comboLanguage.GetCount(); ++i)
		{
			if (static_cast<int>(m_comboLanguage.GetItemData(i)) == 0)
			{
				m_comboLanguage.SetCurSel(i);
				break;
			}
		}
		LoadAlphabet(0);
	}

	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.Activate(TRUE);
	for (const auto& entry : m_specialButtonToolTips)
	{
		CWnd* pWnd = GetDlgItem(entry.first);
		if (pWnd != nullptr)
		{
			m_toolTip.AddTool(pWnd, entry.second);
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CForeignKeyboardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CForeignKeyboardDlg::OnPaint()
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
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CForeignKeyboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// --------------------------
// Aakkostojen alustaminen
// --------------------------
void CForeignKeyboardDlg::InitAlphabets()
{
	m_alphabets.clear();

	// --- Kreikka ---
	{
		Alphabet greek;
		greek.name = L"Greek";

		// Unicode-esimerkit: \u03B1 = α, \u03B2 = β, \u03B3 = γ, \u03C0 = π, \u03C9 = ω
		greek.lowercase = {
			L"\u03B1", L"\u03B2", L"\u03B3", L"\u03B4", L"\u03B5", L"\u03B6", L"\u03B7", L"\u03B8",
			L"\u03B9", L"\u03BA", L"\u03BB", L"\u03BC", L"\u03BD", L"\u03BE", L"\u03BF",
			L"\u03C0", L"\u03C1", L"\u03C3", L"\u03C4", L"\u03C5", L"\u03C6", L"\u03C7",
			L"\u03C8", L"\u03C9"
		};

		// Unicode-esimerkit: \u0391 = Α, \u0392 = Β, \u0393 = Γ, \u03A0 = Π, \u03A9 = Ω
		greek.uppercase = {
			L"\u0391", L"\u0392", L"\u0393", L"\u0394", L"\u0395", L"\u0396", L"\u0397", L"\u0398",
			L"\u0399", L"\u039A", L"\u039B", L"\u039C", L"\u039D", L"\u039E", L"\u039F",
			L"\u03A0", L"\u03A1", L"\u03A3", L"\u03A4", L"\u03A5", L"\u03A6", L"\u03A7",
			L"\u03A8", L"\u03A9"
		};

		greek.charNames = {
			L"Alpha", L"Beta", L"Gamma", L"Delta", L"Epsilon", L"Zeta", L"Eta", L"Theta",
			L"Iota", L"Kappa", L"Lambda", L"Mu", L"Nu", L"Xi", L"Omicron",
			L"Pi", L"Rho", L"Sigma", L"Tau", L"Upsilon", L"Phi", L"Chi",
			L"Psi", L"Omega"
		};

		// Polytonisia merkkejä Isä meidän -rukousta varten (antiikin kreikka)
		// Loppusigma
		greek.lowercase.push_back(L"\u03C2");  // ς
		greek.uppercase.push_back(L"\u03C2");
		greek.charNames.push_back(L"Final Sigma");
		// Alpha-muodot
		greek.lowercase.push_back(L"\u03AC");  // ά
		greek.uppercase.push_back(L"\u0386");  // Ά
		greek.charNames.push_back(L"Alpha with Acute");
		greek.lowercase.push_back(L"\u1F70");  // ὰ
		greek.uppercase.push_back(L"\u1FBA");  // Ὰ
		greek.charNames.push_back(L"Alpha with Grave");
		greek.lowercase.push_back(L"\u1FB6");  // ᾶ
		greek.uppercase.push_back(L"\u1FB6");
		greek.charNames.push_back(L"Alpha with Perispomeni");
		greek.lowercase.push_back(L"\u1F00");  // ἀ
		greek.uppercase.push_back(L"\u1F08");  // Ἀ
		greek.charNames.push_back(L"Alpha with Smooth Breathing");
		greek.lowercase.push_back(L"\u1F01");  // ἁ
		greek.uppercase.push_back(L"\u1F09");  // Ἁ
		greek.charNames.push_back(L"Alpha with Rough Breathing");
		greek.lowercase.push_back(L"\u1F04");  // ἄ
		greek.uppercase.push_back(L"\u1F0C");  // Ἄ
		greek.charNames.push_back(L"Alpha with Smooth Breathing and Acute");
		// Epsilon-muodot
		greek.lowercase.push_back(L"\u03AD");  // έ
		greek.uppercase.push_back(L"\u0388");  // Έ
		greek.charNames.push_back(L"Epsilon with Acute");
		greek.lowercase.push_back(L"\u1F10");  // ἐ
		greek.uppercase.push_back(L"\u1F18");  // Ἐ
		greek.charNames.push_back(L"Epsilon with Smooth Breathing");
		// Eta-muodot
		greek.lowercase.push_back(L"\u03AE");  // ή
		greek.uppercase.push_back(L"\u0389");  // Ή
		greek.charNames.push_back(L"Eta with Acute");
		greek.lowercase.push_back(L"\u1F74");  // ὴ
		greek.uppercase.push_back(L"\u1FCA");  // Ὴ
		greek.charNames.push_back(L"Eta with Grave");
		greek.lowercase.push_back(L"\u1FC6");  // ῆ
		greek.uppercase.push_back(L"\u1FC6");
		greek.charNames.push_back(L"Eta with Perispomeni");
		greek.lowercase.push_back(L"\u1F21");  // ἡ
		greek.uppercase.push_back(L"\u1F29");  // Ἡ
		greek.charNames.push_back(L"Eta with Rough Breathing");
		greek.lowercase.push_back(L"\u1FC3");  // ῃ
		greek.uppercase.push_back(L"\u1FC3");
		greek.charNames.push_back(L"Eta with Iota Subscript");
		// Iota-muodot
		greek.lowercase.push_back(L"\u03AF");  // ί
		greek.uppercase.push_back(L"\u038A");  // Ί
		greek.charNames.push_back(L"Iota with Acute");
		greek.lowercase.push_back(L"\u1F76");  // ὶ
		greek.uppercase.push_back(L"\u1FDA");  // Ὶ
		greek.charNames.push_back(L"Iota with Grave");
		greek.lowercase.push_back(L"\u1FD6");  // ῖ
		greek.uppercase.push_back(L"\u1FD6");
		greek.charNames.push_back(L"Iota with Perispomeni");
		greek.lowercase.push_back(L"\u1F30");  // ἰ
		greek.uppercase.push_back(L"\u1F38");  // Ἰ
		greek.charNames.push_back(L"Iota with Smooth Breathing");
		// Omicron-muodot
		greek.lowercase.push_back(L"\u03CC");  // ό
		greek.uppercase.push_back(L"\u038C");  // Ό
		greek.charNames.push_back(L"Omicron with Acute");
		greek.lowercase.push_back(L"\u1F78");  // ὸ
		greek.uppercase.push_back(L"\u1FF8");  // Ὸ
		greek.charNames.push_back(L"Omicron with Grave");
		greek.lowercase.push_back(L"\u1F40");  // ὀ
		greek.uppercase.push_back(L"\u1F48");  // Ὀ
		greek.charNames.push_back(L"Omicron with Smooth Breathing");
		greek.lowercase.push_back(L"\u1F41");  // ὁ
		greek.uppercase.push_back(L"\u1F49");  // Ὁ
		greek.charNames.push_back(L"Omicron with Rough Breathing");
		greek.lowercase.push_back(L"\u1F44");  // ὄ
		greek.uppercase.push_back(L"\u1F4C");  // Ὄ
		greek.charNames.push_back(L"Omicron with Smooth Breathing and Acute");
		// Upsilon-muodot
		greek.lowercase.push_back(L"\u03CD");  // ύ
		greek.uppercase.push_back(L"\u038E");  // Ύ
		greek.charNames.push_back(L"Upsilon with Acute");
		greek.lowercase.push_back(L"\u1F50");  // ὐ
		greek.uppercase.push_back(L"\u1F50");
		greek.charNames.push_back(L"Upsilon with Smooth Breathing");
		greek.lowercase.push_back(L"\u1FE6");  // ῦ
		greek.uppercase.push_back(L"\u1FE6");
		greek.charNames.push_back(L"Upsilon with Perispomeni");
		// Omega-muodot
		greek.lowercase.push_back(L"\u03CE");  // ώ
		greek.uppercase.push_back(L"\u038F");  // Ώ
		greek.charNames.push_back(L"Omega with Acute");
		greek.lowercase.push_back(L"\u1F61");  // ὡ
		greek.uppercase.push_back(L"\u1F69");  // Ὡ
		greek.charNames.push_back(L"Omega with Rough Breathing");
		greek.lowercase.push_back(L"\u1FF6");  // ῶ
		greek.uppercase.push_back(L"\u1FF6");
		greek.charNames.push_back(L"Omega with Perispomeni");
		greek.lowercase.push_back(L"\u1FF7");  // ῷ
		greek.uppercase.push_back(L"\u1FF7");
		greek.charNames.push_back(L"Omega with Perispomeni and Iota Subscript");
		// Rho-muoto
		greek.lowercase.push_back(L"\u1FE5");  // ῥ
		greek.uppercase.push_back(L"\u1FEC");  // Ῥ
		greek.charNames.push_back(L"Rho with Rough Breathing");

		greek.hasUppercase = true;
		greek.rightToLeft = false;

		m_alphabets.push_back(greek);
	}

	// --- Venäjä ---
	{
		Alphabet russian;
		russian.name = L"Russian";
		// Unicode-esimerkit: \u0430 = а, \u0431 = б, \u0436 = ж, \u044F = я
		russian.lowercase = {
			L"\u0430", L"\u0431", L"\u0432", L"\u0433", L"\u0434", L"\u0435", L"\u0451", L"\u0436",
			L"\u0437", L"\u0438", L"\u0439", L"\u043A", L"\u043B", L"\u043C", L"\u043D",
			L"\u043E", L"\u043F", L"\u0440", L"\u0441", L"\u0442", L"\u0443", L"\u0444",
			L"\u0445", L"\u0446", L"\u0447", L"\u0448", L"\u0449", L"\u044A", L"\u044B",
			L"\u044C", L"\u044D", L"\u044E", L"\u044F"
		};
		// Unicode-esimerkit: \u0410 = А, \u0411 = Б, \u0416 = Ж, \u042F = Я
		russian.uppercase = {
			L"\u0410", L"\u0411", L"\u0412", L"\u0413", L"\u0414", L"\u0415", L"\u0401", L"\u0416",
			L"\u0417", L"\u0418", L"\u0419", L"\u041A", L"\u041B", L"\u041C", L"\u041D",
			L"\u041E", L"\u041F", L"\u0420", L"\u0421", L"\u0422", L"\u0423", L"\u0424",
			L"\u0425", L"\u0426", L"\u0427", L"\u0428", L"\u0429", L"\u042A", L"\u042B",
			L"\u042C", L"\u042D", L"\u042E", L"\u042F"
		};
		russian.charNames = {
			L"A", L"Be", L"Ve", L"Ge", L"De", L"Ye", L"Yo", L"Zhe",
			L"Ze", L"I", L"Short I", L"Ka", L"El", L"Em", L"En",
			L"O", L"Pe", L"Er", L"Es", L"Te", L"U", L"Ef",
			L"Kha", L"Tse", L"Che", L"Sha", L"Shcha", L"Hard Sign", L"Yeru",
			L"Soft Sign", L"E", L"Yu", L"Ya"
		};
		russian.hasUppercase = true;
		russian.rightToLeft = false;
		m_alphabets.push_back(russian);
	}

	// --- Arabia (peruskirjaimet, ilman kaikkia muotoja) ---
	{
		Alphabet arabic;
		arabic.name = L"Arabic";
		// Unicode-esimerkit: \u0627 = ا, \u0628 = ب, \u0633 = س, \u064A = ي
		arabic.lowercase = {
			L"\u0627", L"\u0628", L"\u062A", L"\u062B", L"\u062C", L"\u062D", L"\u062E",
			L"\u062F", L"\u0630", L"\u0631", L"\u0632", L"\u0633", L"\u0634", L"\u0635",
			L"\u0636", L"\u0637", L"\u0638", L"\u0639", L"\u063A", L"\u0641", L"\u0642",
			L"\u0643", L"\u0644", L"\u0645", L"\u0646", L"\u0647", L"\u0648", L"\u064A"
		};

		arabic.charNames = {
			L"Alif", L"Ba", L"Ta", L"Tha", L"Jim", L"Ha", L"Kha",
			L"Dal", L"Dhal", L"Ra", L"Zayn", L"Sin", L"Shin", L"Sad",
			L"Dad", L"Ta (emphatic)", L"Dha", L"Ayn", L"Ghayn", L"Fa", L"Qaf",
			L"Kaf", L"Lam", L"Mim", L"Nun", L"Ha", L"Waw", L"Ya"
		};
		arabic.uppercase.clear(); // Arabialla ei varsinaisesti isoja kirjaimia, mutta tyhjä lista pitää määritellä
		arabic.hasUppercase = false; // Arabia ei varsinaisesti erota isoja ja pieniä kirjaimia
		arabic.rightToLeft = true;
		m_alphabets.push_back(arabic);
	}

	// --- Heprea (peruskirjaimet) ---
	{
		Alphabet hebrew;
		hebrew.name = L"Hebrew";
		// Unicode-esimerkit: \u05D0 = א, \u05D1 = ב, \u05E9 = ש, \u05EA = ת
		hebrew.lowercase = {
			L"\u05D0", L"\u05D1", L"\u05D2", L"\u05D3", L"\u05D4", L"\u05D5", L"\u05D6",
			L"\u05D7", L"\u05D8", L"\u05D9", L"\u05DB", L"\u05DC", L"\u05DE", L"\u05E0",
			L"\u05E1", L"\u05E2", L"\u05E4", L"\u05E6", L"\u05E7", L"\u05E8",
			L"\u05E9", L"\u05EA"
		};
		hebrew.charNames = {
			L"Alef", L"Bet", L"Gimel", L"Dalet", L"He", L"Vav", L"Zayin",
			L"Het", L"Tet", L"Yod", L"Kaf", L"Lamed", L"Mem", L"Nun",
			L"Samekh", L"Ayin", L"Pe", L"Tsadi", L"Qof", L"Resh",
			L"Shin", L"Tav"
		};
		hebrew.uppercase.clear(); // Heprealla ei varsinaisesti isoja kirjaimia, mutta tyhjä lista pitää määritellä
		hebrew.hasUppercase = false; // Heprea ei varsinaisesti erota isoja ja pieniä kirjaimia
		hebrew.rightToLeft = true;
		m_alphabets.push_back(hebrew);
	}

	// --- Farsi (persia, laajennettu arabialainen aakkosto) ---
	{
		Alphabet farsi;
		farsi.name = L"Farsi (Persian)";
		// Unicode-esimerkit: \u067E = پ, \u0686 = چ, \u0698 = ژ, \u06A9 = ک, \u06AF = گ, \u06CC = ی
		farsi.lowercase = {
			L"\u0627", L"\u0628", L"\u067E", L"\u062A", L"\u062B", L"\u062C", L"\u0686",
			L"\u062D", L"\u062E", L"\u062F", L"\u0630", L"\u0631", L"\u0632", L"\u0698",
			L"\u0633", L"\u0634", L"\u0635", L"\u0636", L"\u0637", L"\u0638", L"\u0639",
			L"\u063A", L"\u0641", L"\u0642", L"\u06A9", L"\u06AF", L"\u0644", L"\u0645",
			L"\u0646", L"\u0648", L"\u0647", L"\u06CC"
		};
		farsi.charNames = {
			L"Alef", L"Be", L"Pe", L"Te", L"Se", L"Jim", L"Che",
			L"He", L"Khe", L"Dal", L"Zal", L"Re", L"Ze", L"Zhe",
			L"Sin", L"Shin", L"Sad", L"Zad", L"Ta", L"Za", L"Eyn",
			L"Gheyn", L"Fe", L"Qaf", L"Kaf", L"Gaf", L"Lam", L"Mim",
			L"Nun", L"Vav", L"He", L"Ye"
		};
		farsi.uppercase.clear(); // Farsilla ei varsinaisesti isoja kirjaimia, mutta tyhjä lista pitää määritellä
		farsi.hasUppercase = false; // Farsi ei varsinaisesti erota isoja ja pieniä kirjaimia
		farsi.rightToLeft = true;
		m_alphabets.push_back(farsi);
	}
} // end of InitAlphabets

// ----------------------------------
// Aakkostojen lataaminen comboboxiin
// ----------------------------------

void CForeignKeyboardDlg::InitLanguageCombo()
{
	m_comboLanguage.ResetContent();
	for (int i = 0; i < static_cast<int>(m_alphabets.size()); ++i)
	{
		int itemIndex = m_comboLanguage.AddString(m_alphabets[i].name);
		if (itemIndex != CB_ERR && itemIndex != CB_ERRSPACE)
		{
			m_comboLanguage.SetItemData(itemIndex, static_cast<DWORD_PTR>(i));
		}
	}
} // end of InitLanguageCombo

// -------------------------------------
// Aakkoston lataaminen käyttöliittymään
// -------------------------------------

void CForeignKeyboardDlg::LoadAlphabet(int index)
{
	if (index < 0 || index >= static_cast<int>(m_alphabets.size()))
		return;

	m_currentAlphabetIndex = index;
	m_shiftOn = false;

	const Alphabet& alpha = m_alphabets[index];

	// Päivitetään shift-napin tila
	m_btnShift.EnableWindow(alpha.hasUppercase);
	
	// m_btnShift.SetCheck(BST_UNCHECKED);
	
	UpdateRtlForAlphabet(alpha);
	DestroyKeyboard();
	// Luodaan näppäimet aakkosten perusteella
	CreateKeyboard(alpha);
	UpdateKeyboardCase(alpha);

	InitKeyMapsForAlphabet(alpha);  // voit täydentää myöhemmin, jos haluat tukea fyysisen näppäimistön syötteitä
} // end of LoadAlphabet

// ---------------------------------------------------------------
// Päivitetään tekstilaatikon RTL/LTR-ominaisuus aakkoston mukaan
// ---------------------------------------------------------------
void CForeignKeyboardDlg::UpdateRtlForAlphabet(const Alphabet& alpha)
{
	LONG exStyle = ::GetWindowLong(m_editOutput.m_hWnd, GWL_EXSTYLE);

	if (alpha.rightToLeft)
	{
		exStyle |= (WS_EX_RIGHT | WS_EX_RTLREADING);
	}
	else
	{
		exStyle &= ~(WS_EX_RIGHT | WS_EX_RTLREADING);
	}
	::SetWindowLong(m_editOutput.m_hWnd, GWL_EXSTYLE, exStyle);
	m_editOutput.Invalidate(); // Pakotetaan uudelleenpiirto, jotta muutos näkyy heti

} // end of UpdateRtlForAlphabet

//----------------------------------------
// Näppäinten luonti aakkosten perusteella
//---------------------------------------- 
// Tuhoa vanhat näppäimet ennen uusien luontia
void CForeignKeyboardDlg::DestroyKeyboard()
{
	for (CButton* pBtn : m_keyButtons)
	{
		if (pBtn && ::IsWindow(pBtn->m_hWnd))
		{
			if (m_toolTip.GetSafeHwnd() != nullptr)
				m_toolTip.DelTool(pBtn);
			pBtn->DestroyWindow();
		}
		delete pBtn;
	}
	m_keyButtons.clear();
} // end of DestroyKeyboard

// Luo näppäimet aakkosten perusteell
void CForeignKeyboardDlg::CreateKeyboard(const Alphabet& alpha)
{
	CRect areaRect;
	GetDlgItem(IDC_STATIC_KEYBOARD_AREA)->GetWindowRect(&areaRect);
	ScreenToClient(&areaRect);

	const int margin = 4;
	const int btnWidth = 32;
	const int btnHeight = 28;
	const int spacingX = 2;
	const int spacingY = 2;

	int x = areaRect.left + margin;
	int y = areaRect.top + margin;

	const std::vector<CString>& chars = alpha.lowercase; // Käytetään pientä aakkostoa näppäinten merkkeinä

	for (size_t i = 0; i < chars.size(); ++i)
	{
		if (x + btnWidth > areaRect.right - margin) // Rivinvaihto, jos ei mahdu
		{
			x = areaRect.left + margin;
			y += btnHeight + spacingY;
		}

		CButton* pBtn = new CButton;
		UINT id = KEY_BUTTON_BASE + static_cast<UINT>(i);

		pBtn->Create(chars[i], 
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
			CRect(x, y, x + btnWidth, y + btnHeight), 
			this, 
			id);

		m_keyButtons.push_back(pBtn);

		// Tooltip: merkin nimi
		if (m_toolTip.GetSafeHwnd() != nullptr && i < alpha.charNames.size() && !alpha.charNames[i].IsEmpty())
		{
			m_toolTip.AddTool(pBtn, alpha.charNames[i]);
		}

		x += btnWidth + spacingX;
	}
} // end of CreateKeyboard

//-----------------------------------
// Näppäimistön päivitys
//-----------------------------------
void CForeignKeyboardDlg::UpdateKeyboardCase(const Alphabet& alpha)
{
	const std::vector<CString>* pChars = &alpha.lowercase;

	if (alpha.hasUppercase && m_shiftOn)
	{
		pChars = &alpha.uppercase;
	}

	size_t count = min(m_keyButtons.size(), pChars->size());

	for (size_t i = 0; i < count; ++i)
	{
		if (m_keyButtons[i] && ::IsWindow(m_keyButtons[i]->m_hWnd))
		{
			m_keyButtons[i]->SetWindowText((*pChars)[i]);
		}
	}
} // end of UpdateKeyboardCase

// ----------------------------------------
// Tekstin käsittely näppäinpainalluksista
// ----------------------------------------
void CForeignKeyboardDlg::AppendText(const CString& text)	
{
	int len = m_editOutput.GetWindowTextLengthW();
	m_editOutput.SetSel(len, len); // Asetetaan kursori tekstin loppuun
	m_editOutput.ReplaceSel(text); // Lisätään teksti nykyisen tekstin jälkeen
	/*CString currentText;
	m_editOutput.GetWindowText(currentText);
	currentText += text;
	m_editOutput.SetWindowText(currentText);*/
} // end of AppendText

// ---------------------------------------------------------------------
// Fyysisen näppäimistön syötteiden käsittely (valinnainen jatkokehitys)
// ---------------------------------------------------------------------
void CForeignKeyboardDlg::InitKeyMapsForAlphabet(const Alphabet& alpha)
{
	m_keyMapLower.clear();
	m_keyMapUpper.clear();

	std::vector<UINT> virtualKeys;
	for (UINT vk = 'A'; vk <= 'Z'; ++vk)
	{
		virtualKeys.push_back(vk);
	}

	for (UINT vk = '0'; vk <= '9'; ++vk)
	{
		virtualKeys.push_back(vk);
	}

	const size_t count = min(alpha.lowercase.size(), virtualKeys.size());
	for (size_t i = 0; i < count; ++i)
	{
		const UINT vk = virtualKeys[i];
		m_keyMapLower[vk] = alpha.lowercase[i];

		if (alpha.hasUppercase && i < alpha.uppercase.size())
		{
			m_keyMapUpper[vk] = alpha.uppercase[i];
		}
		else
		{
			m_keyMapUpper[vk] = alpha.lowercase[i];
		}
	}
} // end of InitKeyMapsForAlphabet

// ----------------------------------------
// Viestinkäsittely
// ----------------------------------------
void CForeignKeyboardDlg::OnCbnSelchangeComboLanguage()
{
	int sel = m_comboLanguage.GetCurSel();
	if (sel != CB_ERR)
	{
		DWORD_PTR alphabetIndex = m_comboLanguage.GetItemData(sel);
		if (alphabetIndex != CB_ERR)
		{
			LoadAlphabet(static_cast<int>(alphabetIndex));
		}
	}
} // end of OnCbnSelchangeComboLanguage

// Shift-napin käsittely
void CForeignKeyboardDlg::OnBnClickedButtonShift()
{
	if (m_currentAlphabetIndex < 0 || m_currentAlphabetIndex >= static_cast<int>(m_alphabets.size()))
		return;

	m_shiftOn = !m_shiftOn;
	UpdateKeyboardCase(m_alphabets[m_currentAlphabetIndex]);

	CString caption = m_shiftOn ? L"Shift ON" : L"Shift OFF";
	m_btnShift.SetWindowText(caption);
} // end of OnBnClickedButtonShift

// Dynaamisten näppäinten käsittely
void CForeignKeyboardDlg::OnKeyboardButton(UINT nID)
{
	if (m_currentAlphabetIndex < 0 || m_currentAlphabetIndex >= static_cast<int>(m_alphabets.size()))
		return;

	size_t index = nID - KEY_BUTTON_BASE;
	
	const Alphabet& alpha = m_alphabets[m_currentAlphabetIndex];

	const std::vector<CString>* pChars = &alpha.lowercase;

	if (alpha.hasUppercase && m_shiftOn)
	{
		pChars = &alpha.uppercase;
	}
	if (index >= 0 && index < static_cast<int>(pChars->
		size()))
	{
		AppendText((*pChars)[index]);
	}
} // end of OnKeyboardButton

// --------------------------------------------------------------------------------------------
// PreTranslateMessage - fyysisen näppäimistön syötteiden käsittely (valinnainen jatkokehitys)
// --------------------------------------------------------------------------------------------
BOOL CForeignKeyboardDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_toolTip.m_hWnd != nullptr)
		m_toolTip.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		UINT vk = static_cast<UINT>(pMsg->wParam);
		bool shiftPressed = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
		auto itLower = m_keyMapLower.find(vk);
		if (itLower != m_keyMapLower.end())
		{
			CString ch = shiftPressed ? m_keyMapUpper[vk] : itLower->second;
			AppendText(ch);
			return TRUE; // Viestintä käsitelty
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
} // end of PreTranslateMessage

// -------------------------------------------------------
// Erikoisnäppäinten tooltip-tekstien alustaminen
// -------------------------------------------------------
void CForeignKeyboardDlg::InitSpecialButtonToolTips()
{
	m_specialButtonToolTips[IDC_BUTTON_COPY]                = L"Copy all text to clipboard";
	m_specialButtonToolTips[IDC_BUTTON_CLEAR]               = L"Clear all text";
	m_specialButtonToolTips[IDC_BUTTON_SPACE]               = L"Space";
	m_specialButtonToolTips[IDC_BUTTON_PERIOD]              = L"Period, dot, or full stop .";
	m_specialButtonToolTips[IDC_BUTTON_COMMA]               = L"Comma ,";
	m_specialButtonToolTips[IDC_BUTTON_SEMICOLON]           = L"Semicolon ;";
	m_specialButtonToolTips[IDC_BUTTON_COLON]               = L"Colon :";
	m_specialButtonToolTips[IDC_BUTTON_QUOTATION]           = L"Quotation mark \"";
	m_specialButtonToolTips[IDC_BUTTON_SINGLE_QUOTATION]    = L"Single quotation mark '";
	m_specialButtonToolTips[IDC_BUTTON_DASH]                = L"Hyphen, minus sign or dash -";
	m_specialButtonToolTips[IDC_BUTTON_UNDERBAR]            = L"Underscore or underbar _";
	m_specialButtonToolTips[IDC_BUTTON_ACUTE_ACCENT]        = L"Acute accent \u00B4";
	m_specialButtonToolTips[IDC_BUTTON_GRAVE_ACCENT]        = L"Grave accent `";
	m_specialButtonToolTips[IDC_BUTTON_CIRCUMFLEX]          = L"Circumflex or caret ^";
	m_specialButtonToolTips[IDC_BUTTON_TREMA]               = L"Trema \u00A8";
	m_specialButtonToolTips[IDC_BUTTON_TILDE]               = L"Tilde ~";
	m_specialButtonToolTips[IDC_BUTTON_ASTERISK]            = L"Asterisk *";
	m_specialButtonToolTips[IDC_BUTTON_EXCLAMATION]         = L"Exclamation mark !";
	m_specialButtonToolTips[IDC_BUTTON_QUESTION]            = L"Question mark ?";
	m_specialButtonToolTips[IDC_BUTTON_BACKSPACE]           = L"Backspace - delete character before cursor";
	m_specialButtonToolTips[IDC_BUTTON_DEL]                 = L"Delete - delete character after cursor";
	m_specialButtonToolTips[IDC_BUTTON_OCTOTHORPE]          = L"Octothorpe, number or hash sign #";
	m_specialButtonToolTips[IDC_BUTTON_CURRENCY_SIGN]       = L"Currency sign \u00A4";
	m_specialButtonToolTips[IDC_BUTTON_PERCENT]             = L"Percent sign %";
	m_specialButtonToolTips[IDC_BUTTON_AMPERSAND]           = L"Ampersand &";
	m_specialButtonToolTips[IDC_BUTTON_SOLIDUS]             = L"Forward slash /";
	m_specialButtonToolTips[IDC_BUTTON_LEFT_PARENTHESIS]    = L"Open parenthesis (";
	m_specialButtonToolTips[IDC_BUTTON_RIGHT_PARENTHESIS]   = L"Close parenthesis )";
	m_specialButtonToolTips[IDC_BUTTON_EQUAL_SIGN]          = L"Equal sign =";
	m_specialButtonToolTips[IDC_BUTTON_PLUS_SIGN]           = L"Plus sign +";
	m_specialButtonToolTips[IDC_BUTTON_REVERSE_SOLIDUS]     = L"Backslash \\";
	m_specialButtonToolTips[IDC_BUTTON_AMPERSAT]            = L"At sign @";
	m_specialButtonToolTips[IDC_BUTTON_POUND_SIGN]          = L"Pound sterling \u00A3";
	m_specialButtonToolTips[IDC_BUTTON_DOLLAR_SIGN]         = L"Dollar sign $";
	m_specialButtonToolTips[IDC_BUTTON_EURO_SIGN]           = L"Euro sign \u20AC";
	m_specialButtonToolTips[IDC_BUTTON_LEFT_BRACE]          = L"Open curly brace {";
	m_specialButtonToolTips[IDC_BUTTON_RIGHT_BRACE]         = L"Close curly brace }";
	m_specialButtonToolTips[IDC_BUTTON_LEFT_BRACKET]        = L"Open bracket [";
	m_specialButtonToolTips[IDC_BUTTON_RIGHT_BRACKET]       = L"Close bracket ]";
	m_specialButtonToolTips[IDC_BUTTON_CENT_SIGN]           = L"Cent sign \u00A2";
	m_specialButtonToolTips[IDC_BUTTON_YUAN_SIGN]           = L"Yen/Yuan sign \u00A5";
	m_specialButtonToolTips[IDC_BUTTON_SECTION_SYMBOL]      = L"Section sign \u00A7";
	m_specialButtonToolTips[IDC_BUTTON_HOME_KEY]            = L"Home - move cursor to beginning";
	m_specialButtonToolTips[IDC_BUTTON_END_KEY]             = L"End - move cursor to end";
	m_specialButtonToolTips[IDC_BUTTON_LESS_THAN]           = L"Less than <";
	m_specialButtonToolTips[IDC_BUTTON_GREATER_THAN]        = L"Greater than >";
	m_specialButtonToolTips[IDC_BUTTON_PIPE]                = L"Vertical bar or pipe |";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_ZERO]         = L"Zero 0";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_ONE]          = L"One 1";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_TWO]          = L"Two 2";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_THREE]        = L"Three 3";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_FOUR]         = L"Four 4";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_FIVE]         = L"Five 5";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_SIX]          = L"Six 6";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_SEVEN]        = L"Seven 7";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_EIGHT]        = L"Eight 8";
	m_specialButtonToolTips[IDC_BUTTON_NUMBER_NINE]         = L"Nine 9";
	m_specialButtonToolTips[IDC_BUTTON_ONEHALF]             = L"One half \u00BD";
} // end of InitSpecialButtonToolTips

// -------------------------------------------------------
// Palauttaa erikoisnäppäimelle lisättävän merkin
// -------------------------------------------------------
CString CForeignKeyboardDlg::GetSpecialButtonInsertText(UINT nID) const
{
	switch (nID)
	{
	case IDC_BUTTON_SPACE:              return L" ";
	case IDC_BUTTON_PERIOD:             return L".";
	case IDC_BUTTON_COMMA:              return L",";
	case IDC_BUTTON_SEMICOLON:          return L";";
	case IDC_BUTTON_COLON:              return L":";
	case IDC_BUTTON_QUOTATION:          return L"\"";
	case IDC_BUTTON_SINGLE_QUOTATION:   return L"'";
	case IDC_BUTTON_DASH:               return L"-";
	case IDC_BUTTON_UNDERBAR:           return L"_";
	case IDC_BUTTON_ACUTE_ACCENT:       return L"\u00B4";
	case IDC_BUTTON_GRAVE_ACCENT:       return L"`";
	case IDC_BUTTON_CIRCUMFLEX:         return L"^";
	case IDC_BUTTON_TREMA:              return L"\u00A8";
	case IDC_BUTTON_TILDE:              return L"~";
	case IDC_BUTTON_ASTERISK:           return L"*";
	case IDC_BUTTON_EXCLAMATION:        return L"!";
	case IDC_BUTTON_QUESTION:           return L"?";
	case IDC_BUTTON_OCTOTHORPE:         return L"#";
	case IDC_BUTTON_CURRENCY_SIGN:      return L"\u00A4";
	case IDC_BUTTON_PERCENT:            return L"%";
	case IDC_BUTTON_AMPERSAND:          return L"&";
	case IDC_BUTTON_SOLIDUS:            return L"/";
	case IDC_BUTTON_LEFT_PARENTHESIS:   return L"(";
	case IDC_BUTTON_RIGHT_PARENTHESIS:  return L")";
	case IDC_BUTTON_EQUAL_SIGN:         return L"=";
	case IDC_BUTTON_PLUS_SIGN:          return L"+";
	case IDC_BUTTON_REVERSE_SOLIDUS:    return L"\\";
	case IDC_BUTTON_AMPERSAT:           return L"@";
	case IDC_BUTTON_POUND_SIGN:         return L"\u00A3";
	case IDC_BUTTON_DOLLAR_SIGN:        return L"$";
	case IDC_BUTTON_EURO_SIGN:          return L"\u20AC";
	case IDC_BUTTON_LEFT_BRACE:         return L"{";
	case IDC_BUTTON_RIGHT_BRACE:        return L"}";
	case IDC_BUTTON_LEFT_BRACKET:       return L"[";
	case IDC_BUTTON_RIGHT_BRACKET:      return L"]";
	case IDC_BUTTON_CENT_SIGN:          return L"\u00A2";
	case IDC_BUTTON_YUAN_SIGN:          return L"\u00A5";
	case IDC_BUTTON_SECTION_SYMBOL:     return L"\u00A7";
	case IDC_BUTTON_LESS_THAN:          return L"<";
	case IDC_BUTTON_GREATER_THAN:       return L">";
	case IDC_BUTTON_PIPE:               return L"|";
	case IDC_BUTTON_NUMBER_ZERO:        return L"0";
	case IDC_BUTTON_NUMBER_ONE:         return L"1";
	case IDC_BUTTON_NUMBER_TWO:         return L"2";
	case IDC_BUTTON_NUMBER_THREE:       return L"3";
	case IDC_BUTTON_NUMBER_FOUR:        return L"4";
	case IDC_BUTTON_NUMBER_FIVE:        return L"5";
	case IDC_BUTTON_NUMBER_SIX:         return L"6";
	case IDC_BUTTON_NUMBER_SEVEN:       return L"7";
	case IDC_BUTTON_NUMBER_EIGHT:       return L"8";
	case IDC_BUTTON_NUMBER_NINE:        return L"9";
	case IDC_BUTTON_ONEHALF:            return L"\u00BD";
	default:                            return L"";
	}
} // end of GetSpecialButtonInsertText

// -------------------------------------------------------
// Lisää tekstin kursorin kohtaan editissä
// -------------------------------------------------------
void CForeignKeyboardDlg::InsertTextAtCursor(const CString& text)
{
	m_editOutput.ReplaceSel(text, TRUE);
} // end of InsertTextAtCursor

// -------------------------------------------------------
// Kopioi kaikki teksti leikepöydälle
// -------------------------------------------------------
void CForeignKeyboardDlg::CopyOutputText()
{
	CString text;
	m_editOutput.GetWindowText(text);
	if (text.IsEmpty())
		return;

	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();
		size_t size = static_cast<size_t>(text.GetLength() + 1) * sizeof(WCHAR);
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, size);
		if (hMem)
		{
			LPWSTR pData = static_cast<LPWSTR>(::GlobalLock(hMem));
			if (pData)
			{
				wcscpy_s(pData, text.GetLength() + 1, static_cast<LPCWSTR>(text));
				::GlobalUnlock(hMem);
				::SetClipboardData(CF_UNICODETEXT, hMem);
			}
		}
		::CloseClipboard();
	}
} // end of CopyOutputText

// -------------------------------------------------------
// Poistaa kursorin edellä olevan merkin (Backspace)
// -------------------------------------------------------
void CForeignKeyboardDlg::BackspaceOutputText()
{
	int nStart = 0, nEnd = 0;
	m_editOutput.GetSel(nStart, nEnd);
	if (nStart != nEnd)
	{
		m_editOutput.ReplaceSel(L"", TRUE);
	}
	else if (nStart > 0)
	{
		m_editOutput.SetSel(nStart - 1, nEnd);
		m_editOutput.ReplaceSel(L"", TRUE);
	}
} // end of BackspaceOutputText

// -------------------------------------------------------
// Poistaa kursorin jälkeisen merkin (Delete)
// -------------------------------------------------------
void CForeignKeyboardDlg::DeleteOutputText()
{
	int nStart = 0, nEnd = 0;
	m_editOutput.GetSel(nStart, nEnd);
	if (nStart != nEnd)
	{
		m_editOutput.ReplaceSel(L"", TRUE);
	}
	else
	{
		int len = m_editOutput.GetWindowTextLength();
		if (nStart < len)
		{
			m_editOutput.SetSel(nStart, nStart + 1);
			m_editOutput.ReplaceSel(L"", TRUE);
		}
	}
} // end of DeleteOutputText

// -------------------------------------------------------
// Siirtää kursorin tekstin alkuun (Home)
// -------------------------------------------------------
void CForeignKeyboardDlg::MoveCaretToStart()
{
	m_editOutput.SetSel(0, 0);
} // end of MoveCaretToStart

// -------------------------------------------------------
// Siirtää kursorin tekstin loppuun (End)
// -------------------------------------------------------
void CForeignKeyboardDlg::MoveCaretToEnd()
{
	int len = m_editOutput.GetWindowTextLength();
	m_editOutput.SetSel(len, len);
} // end of MoveCaretToEnd

// -------------------------------------------------------
// Erikoisnäppäimien painallustenkäsittely
// -------------------------------------------------------
void CForeignKeyboardDlg::OnSpecialButton(UINT nID)
{
	switch (nID)
	{
	case IDC_BUTTON_COPY:
		CopyOutputText();
		break;
	case IDC_BUTTON_CLEAR:
		m_editOutput.SetWindowText(L"");
		break;
	case IDC_BUTTON_BACKSPACE:
		BackspaceOutputText();
		break;
	case IDC_BUTTON_DEL:
		DeleteOutputText();
		break;
	case IDC_BUTTON_HOME_KEY:
		MoveCaretToStart();
		break;
	case IDC_BUTTON_END_KEY:
		MoveCaretToEnd();
		break;
	default:
	{
		CString text = GetSpecialButtonInsertText(nID);
		if (!text.IsEmpty())
			InsertTextAtCursor(text);
		break;
	}
	}
} // end of OnSpecialButton



void CForeignKeyboardDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

// -------------------------------------------------------
// Help-painikkeen käsittely - avaa CHM-ohjetiedoston
// -------------------------------------------------------
void CForeignKeyboardDlg::OnBnClickedButtonHelp()
{
	TCHAR szPath[MAX_PATH] = {};
	::GetModuleFileName(nullptr, szPath, MAX_PATH);
	TCHAR* pSlash = _tcsrchr(szPath, _T('\\'));
	if (pSlash)
		*(pSlash + 1) = _T('\0');
	CString helpPath = CString(szPath) + _T("ForeignKeyboard.chm");

	HWND hWnd = ::HtmlHelp(m_hWnd, helpPath, HH_DISPLAY_TOC, 0);
	if (hWnd == nullptr)
	{
		MessageBox(
			_T("Help file not found.\n\nPlease ensure ForeignKeyboard.chm is in the same folder as the application."),
			_T("Help"),
			MB_OK | MB_ICONINFORMATION);
	}
} // end of OnBnClickedButtonHelp

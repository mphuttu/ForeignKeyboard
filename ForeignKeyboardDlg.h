
// ForeignKeyboardDlg.h : header file
//

#pragma once
#include <vector>
#include <map>

struct Alphabet
{
	CString name;       // Näytettävä nimi comboboxissa
	std::vector<CString> lowercase; // Pienet kirjaimet / perusmerkit
	std::vector<CString> uppercase; // Isot kirjaimet (jos on)
	std::vector<CString> charNames; // Merkkien nimet tooltip-tekstejä varten
	bool hasUppercase;   // Onko isoja kirjaimia, true = esim. kreikka, venäjä
	bool rightToLeft;     // Onko oikealta vasemmalle kirjoitettava kieli, true = esim. arabia, heprea, farsi
};

#define KEY_BUTTON_BASE 50000
#define KEY_BUTTON_MAX  51000


// CForeignKeyboardDlg dialog
class CForeignKeyboardDlg : public CDialogEx
{
// Construction
public:
	CForeignKeyboardDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOREIGNKEYBOARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// UI-kontrollit
	CComboBox m_comboLanguage;
	CEdit m_editOutput;
	CButton m_btnShift;
	CToolTipCtrl m_toolTip;
	
	// Aakkostot
	std::vector<Alphabet> m_alphabets;
	int m_currentAlphabetIndex;
	bool m_shiftOn;

	// Dynaamiset näppäimet
	std::vector<CButton*> m_keyButtons; // Dynaamiset näppäimet

	// Fyysisen näppäimistön tilan seuranta (valinnain jatkokehitys)
	std::map<UINT, CString> m_keyMapLower; // Mapataan näppäin ID:llä sen merkkiin
	std::map<UINT, CString> m_keyMapUpper; // Mapataan näppäin ID:llä sen merkkiin shiftillä
	std::map<UINT, CString> m_specialButtonToolTips;

private:
	// Alustukset
	void InitAlphabets();
	void InitLanguageCombo();
	void InitSpecialButtonToolTips();
	void LoadAlphabet(int index);
	void UpdateRtlForAlphabet(const Alphabet& alpha);

	// Näppäinten luonti ja päivitys
	void CreateKeyboard(const Alphabet& alpha);
	void DestroyKeyboard();
	void UpdateKeyboardCase(const Alphabet& alpha);

	// Tekstinkäsittely
	void AppendText(const CString& ch);
	void InsertTextAtCursor(const CString& text);
	void CopyOutputText();
	void BackspaceOutputText();
	void DeleteOutputText();
	void MoveCaretToStart();
	void MoveCaretToEnd();
	CString GetSpecialButtonInsertText(UINT nID) const;

	// Fyysisen näppäimistön mapit (voit täydentää myöhemmin)
	void InitKeyMapsForAlphabet(const Alphabet& alpha);

public:
	afx_msg void OnCbnSelchangeComboLanguage();
	afx_msg void OnBnClickedButtonShift();
	afx_msg void OnKeyboardButton(UINT nID);
	afx_msg void OnSpecialButton(UINT nID);
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonHelp();
};

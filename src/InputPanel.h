#pragma once

#include <d2d1.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wrl/client.h>

#include <string>
#include <vector>

#include "CCriticalSection.h"

typedef std::vector<std::wstring> CandidateList;

#define SCI_ES_DISABLE 0

// IMEPanel->bEnabled values

// Don't draw UI
#define IME_UI_DISABLED 0L
// Draw UI in game
#define IME_UI_ENABLED 1L

extern RTL_CRITICAL_SECTION g_rcsPanelCriticalSection;

struct PanelPosition
{
	float fX;
	float fY;
	float fWidth;
	float fPadding;
	float fCornor;

	PanelPosition() :
		fX(25.0f), fY(35.0f),
		fPadding(5.0f),
		fCornor(5.0f),
		fWidth(350.0f)
	{}
};

struct PanelFontSizes
{
	float fHeader;
	float fComposition;
	float fCandidate;

	PanelFontSizes() :
		fHeader(18.0f),
		fComposition(15.5f),
		fCandidate(16.0f) {}
};

// Used for render in game IME panel and cache IME informations
class IMEPanel final : public Singleton<IMEPanel>
{
public:
	IMEPanel();

	bool Initialize(IDXGISwapChain* a_pSwapChain);

	void ReloadConfigs();

	void NextGroup();
	void OnResetDevice();
	void OnLostDevice();
	/// Render IME panel
	HRESULT OnRender();
	/// TODO, Triggered when window
	HRESULT OnResizeTarget();

	[[nodiscard]] inline bool Initialized() { return m_bInitialized; }

	volatile BOOL uiState = IME_UI_DISABLED;

	// Whether or not allow special key been processed by Scaleform MessageQueue
	volatile ULONG bDisableSpecialKey = FALSE;
	volatile ULONG bEnabled = FALSE;

	volatile ULONG ulPageStartIndex = 0;
	volatile ULONG ulSlectedIndex = 0;

	/// Name of current method name
	std::wstring wstrInputMethodName;
	/// 输入的内容, 需要调用DrawTextW()来绘制，所以是宽字符
	std::wstring wstrComposition;
	/// 候选字列表
	CandidateList vwsCandidateList;

	HWND hWindow;
	CCriticalSection csImeInformation;

private:
	bool m_bInitialized;

	PanelFontSizes m_fontSizes;
	PanelPosition m_position;
	std::wstring m_wsFontName;

	IDXGISwapChain* m_pSwapChain;

	IDWriteTextFormat* m_pHeaderFormat;
	IDWriteTextFormat* m_pInputContentFormat;
	IDWriteTextFormat* m_pCandicateItemFormat;

	D2D1_ROUNDED_RECT m_rectWidget;
	D2D_RECT_F m_rectHeader;
	D2D_RECT_F m_rectComposition;

	ID2D1SolidColorBrush* m_pBackgroundBrush;
	ID2D1SolidColorBrush* m_pHeaderColorBrush;
	ID2D1SolidColorBrush* m_pInputContentBrush;
	ID2D1SolidColorBrush* m_pCandidateColorBrush;
	ID2D1SolidColorBrush* m_pSelectedColorBrush;

	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWFactory;
	ID2D1RenderTarget* m_pBackBufferRT;

	/// Create Resource of D2D and DirectWrite (Fonts)
	///  创建D2D相关资源，包括字体
	HRESULT CreateD2DResources();

	/// 创建笔刷
	HRESULT CreateBrushes();

	/// 计算位置
	void CalculatePosition();
};
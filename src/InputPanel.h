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

// Used for cache IME informations
class IMEPanel final : public Singleton<IMEPanel>
{
public:
	IMEPanel() = default;

	/// Render IME panel
	void OnRender();
	void Initialize(HWND hWnd);

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

	CCriticalSection csImeInformation;

	HWND hWindow;
};
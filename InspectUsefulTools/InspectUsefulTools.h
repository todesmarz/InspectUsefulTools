/*! @file  InspectUsefulTools.h
    @brief 当ツールアプリケーションクラスヘッダ
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル

/*!
 @class CInspectUsefulToolsApp
 @brief 当ツールアプリケーションクラス
*/
class CInspectUsefulToolsApp : public CWinAppEx
{
public:
	CInspectUsefulToolsApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装
	DECLARE_MESSAGE_MAP()
};

extern CInspectUsefulToolsApp theApp;
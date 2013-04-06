﻿/*! @file  SysErrorDescriptDlg.h
    @brief エラー詳細表示画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonNumExecDlg.h"


/*!
 @struct SErrorInfoData
 @brief  サービス情報構造体
*/
typedef struct SErrorInfoData_ {
	int nErrorNo;		/// エラー番号
	LPCTSTR szDetail;	/// エラー内容
} SErrorInfoData;

static SErrorInfoData SErrorInfoDataTbl[] = {
	//
	// HTTP Status Code
	//
	{100, _T("HTTP : クライアントが要求を継続できることを示します\r\n")},
	{101, _T("HTTP : プロトコル バージョンまたはプロトコルが変更されていることを示します\r\n")},

	{200, _T("HTTP : 要求が成功し、要求された情報が応答で返されたことを示します。\r\n これは受信される最も一般的なステータス コードです\r\n")},
	{201, _T("HTTP : 応答が送信される前に、要求によって新しいリソースが作成されたことを示します\r\n")},
	{202, _T("HTTP : 要求が追加の処理のために受け入れられたことを示します\r\n")},
	{203, _T("HTTP : 返されたメタ情報が、元のサーバではなくキャッシュされたコピーから取得されたもので、正しくない可能性があることを示します\r\n")},
	{204, _T("HTTP : 要求が正常に処理され、応答が意図的に空白になっていることを示します\r\n")},
	{205, _T("HTTP : クライアントが現在のリソースを再び読み込むのではなく、リセットする必要があることを示します\r\n")},
	{206, _T("HTTP : 応答が GET 要求によって要求された応答の一部であることを示します。これにはバイト範囲が含まれます\r\n")},

	{300, _T("HTTP : 要求された情報に複数の表現があることを示します。\r\n 既定のアクションでは、このステータスをリダイレクトとして扱い、この応答と関連付けられた Location ヘッダの内容に従います\r\n")},
	{301, _T("HTTP : 要求された情報が Location ヘッダで指定される URI に移動したことを示します。\r\n このステータスを受信したときの既定のアクションは、応答に関連付けられている Location ヘッダの追跡です。\r\n 元の要求メソッドが POST の場合、リダイレクトされた要求は GET メソッドを使用します\r\n")},
	{302, _T("HTTP : 要求された情報が Location ヘッダで指定される URI にあることを示します。\r\n このステータスを受信したときの既定のアクションは、応答に関連付けられている Location ヘッダの追跡です。\r\n 元の要求メソッドが POST の場合、リダイレクトされた要求は GET メソッドを使用します\r\n")},
	{303, _T("HTTP : POST の結果として、Location ヘッダで指定された URI にクライアントを自動的にリダイレクトします。\r\n Location ヘッダで指定されるリソースへの要求は、GET で行います\r\n")},
	{304, _T("HTTP : クライアントのキャッシュされたコピーが最新のものであることを示します。\r\n リソースの内容は転送されません\r\n")},
	{305, _T("HTTP : 要求が Location ヘッダで指定される URI でプロキシ サーバを使用する必要があることを示します\r\n")},
	{306, _T("HTTP : Unused\r\n")},
	{307, _T("HTTP : 要求された情報が Location ヘッダで指定される URI にあることを示します。\r\n このステータスを受信したときの既定のアクションは、応答に関連付けられている Location ヘッダの追跡です。\r\n 元の要求メソッドが POST の場合、リダイレクトされた要求も POST メソッドを使用します\r\n")},

	{400, _T("HTTP : サーバが要求を理解できないことを示します。\r\n BadRequest は、他のエラーが適用されない場合や、正確なエラーを確認できないか、独自のエラー コードがない場合に送信されます\r\n")},
	{401, _T("HTTP : 要求されたリソースが認証を要求することを示します。\r\n WWW-Authenticate ヘッダには、認証を実行する方法の詳細が格納されます\r\n")},
	{402, _T("HTTP : Payment Required\r\n")},
	{403, _T("HTTP : サーバが要求の実行を拒否していることを示します\r\n")},
	{404, _T("HTTP : 要求されたリソースがサーバに存在していないことを示します\r\n")},
	{405, _T("HTTP : 要求されたリソースがサーバに存在していないことを示します\r\n")},
	{406, _T("HTTP : クライアントが Accept ヘッダでリソースの利用可能な任意の表現を受け入れないことを指定していることを示します\r\n")},
	{407, _T("HTTP : 要求されたプロキシが認証を要求することを示します。\r\n Proxy-authenticate ヘッダには、認証を実行する方法の詳細が格納されます\r\n")},
	{408, _T("HTTP : サーバが要求を予期している時間内にクライアントが要求を送信しなかったことを示します\r\n")},
	{409, _T("HTTP : サーバ上の競合のために要求を実行できないことを示します\r\n")},
	{410, _T("HTTP : 要求されたリソースが利用できないことを示します\r\n")},
	{411, _T("HTTP : 必要な Content-length ヘッダがないことを示します\r\n")},
	{412, _T("HTTP : この要求で設定された条件が満たされず、要求を実行できないことを示します。\r\n 条件は、If-Match、If-None-Match、If-Unmodified-Since などの条件要求ヘッダで設定します\r\n")},
	{413, _T("HTTP : 要求が大きすぎて、サーバで処理できないことを示します\r\n")},
	{414, _T("HTTP : URI が長すぎることを示します\r\n")},
	{415, _T("HTTP : その要求の種類がサポートされていないことを示します\r\n")},
	{416, _T("HTTP : リソースから要求されたデータの範囲を返すことができないことを示します。\r\n その原因は、範囲の先頭がリソースの先頭より前であるか、範囲の末尾がリソースの末尾より後ろであるかのいずれかです\r\n")},
	{417, _T("HTTP : サーバが Expect ヘッダで指定された要求を満たすことができないことを示します\r\n")},

	{500, _T("HTTP : サーバで一般的なエラーが発生したことを示します\r\n")},
	{501, _T("HTTP : サーバが要求された機能をサポートしていないことを示します\r\n")},
	{502, _T("HTTP : 中間プロキシ サーバが別のプロキシまたは元のサーバから無効な応答を受け取ったことを示します\r\n")},
	{503, _T("HTTP : 高い負荷または保守のため、サーバを一時的に利用できないことを示します\r\n")},
	{504, _T("HTTP : 中間プロキシ サーバが別のプロキシまたは元のサーバからの応答を待機している間にタイムアウトしたことを示します\r\n")},
	{505, _T("HTTP : 要求された HTTP バージョンがサーバでサポートされていないことを示します\r\n")},

	{0, NULL}

};

/*!
 @class CSysErrorDescriptDlg
 @brief 処理時間計測画面クラス
*/
class CSysErrorDescriptDlg : public CIUCommonNumExecDlg
{
public:
	CSysErrorDescriptDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	virtual CString GetInformation(BOOL bInit);

protected:
	virtual BOOL OnInitDialog();
};
// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CRtdServer ラッパー クラス

class CRtdServer : public COleDispatchDriver
{
public:
	CRtdServer(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CRtdServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRtdServer(const CRtdServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// IRtdServer メソッド
public:
	long ServerStart(LPDISPATCH CallbackObject)
	{
		long result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, CallbackObject);
		return result;
	}
	VARIANT ConnectData(long TopicID, SAFEARRAY * * Strings, BOOL * GetNewValues)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_UNKNOWN VTS_PBOOL ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, TopicID, Strings, GetNewValues);
		return result;
	}
	SAFEARRAY * RefreshData(long * TopicCount)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TopicCount);
	}
	void DisconnectData(long TopicID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TopicID);
	}
	long Heartbeat()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void ServerTerminate()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// IRtdServer プロパティ
public:

};

// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSort ラッパー クラス

class CSort : public COleDispatchDriver
{
public:
	CSort(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CSort(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSort(const CSort& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Sort メソッド
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Rng()
	{
		LPDISPATCH result;
		InvokeHelper(0xabc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Header()
	{
		long result;
		InvokeHelper(0x37f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Header(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x37f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MatchCase()
	{
		BOOL result;
		InvokeHelper(0x1aa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MatchCase(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1aa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Orientation()
	{
		long result;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SortMethod()
	{
		long result;
		InvokeHelper(0x381, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SortMethod(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x381, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_SortFields()
	{
		LPDISPATCH result;
		InvokeHelper(0xabd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetRange(LPDISPATCH Rng)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xabe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Rng);
	}
	void Apply()
	{
		InvokeHelper(0x68b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// Sort プロパティ
public:

};

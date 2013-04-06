// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CXmlDataBinding ラッパー クラス

class CXmlDataBinding : public COleDispatchDriver
{
public:
	CXmlDataBinding(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CXmlDataBinding(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CXmlDataBinding(const CXmlDataBinding& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// XmlDataBinding メソッド
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
	CString get__Default()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long Refresh()
	{
		long result;
		InvokeHelper(0x589, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void LoadSettings(LPCTSTR Url)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x919, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Url);
	}
	void ClearSettings()
	{
		InvokeHelper(0x91a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_SourceUrl()
	{
		CString result;
		InvokeHelper(0x91b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// XmlDataBinding プロパティ
public:

};

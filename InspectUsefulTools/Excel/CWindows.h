﻿// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CWindows ラッパー クラス

class CWindows : public COleDispatchDriver
{
public:
	CWindows(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CWindows(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWindows(const CWindows& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Windows メソッド
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
	VARIANT Arrange(long ArrangeStyle, VARIANT& ActiveWorkbook, VARIANT& SyncHorizontal, VARIANT& SyncVertical)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x27e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, ArrangeStyle, &ActiveWorkbook, &SyncHorizontal, &SyncVertical);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get__Default(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	BOOL CompareSideBySideWith(VARIANT& WindowName)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x8c6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &WindowName);
		return result;
	}
	BOOL BreakSideBySide()
	{
		BOOL result;
		InvokeHelper(0x8c8, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_SyncScrollingSideBySide()
	{
		BOOL result;
		InvokeHelper(0x8c9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SyncScrollingSideBySide(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8c9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ResetPositionsSideBySide()
	{
		InvokeHelper(0x8ca, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// Windows プロパティ
public:

};

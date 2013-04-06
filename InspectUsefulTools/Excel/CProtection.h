// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CProtection ラッパー クラス

class CProtection : public COleDispatchDriver
{
public:
	CProtection(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CProtection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CProtection(const CProtection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Protection メソッド
public:
	BOOL get_AllowFormattingCells()
	{
		BOOL result;
		InvokeHelper(0x7f0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowFormattingColumns()
	{
		BOOL result;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowFormattingRows()
	{
		BOOL result;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowInsertingColumns()
	{
		BOOL result;
		InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowInsertingRows()
	{
		BOOL result;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowInsertingHyperlinks()
	{
		BOOL result;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowDeletingColumns()
	{
		BOOL result;
		InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowDeletingRows()
	{
		BOOL result;
		InvokeHelper(0x7f7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowSorting()
	{
		BOOL result;
		InvokeHelper(0x7f8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowFiltering()
	{
		BOOL result;
		InvokeHelper(0x7f9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowUsingPivotTables()
	{
		BOOL result;
		InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AllowEditRanges()
	{
		LPDISPATCH result;
		InvokeHelper(0x8bc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// Protection プロパティ
public:

};

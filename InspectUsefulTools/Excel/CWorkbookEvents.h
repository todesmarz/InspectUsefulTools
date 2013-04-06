// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CWorkbookEvents ラッパー クラス

class CWorkbookEvents : public COleDispatchDriver
{
public:
	CWorkbookEvents(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CWorkbookEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWorkbookEvents(const CWorkbookEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// WorkbookEvents メソッド
public:
	void Open()
	{
		InvokeHelper(0x783, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Activate()
	{
		InvokeHelper(0x130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Deactivate()
	{
		InvokeHelper(0x5fa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void BeforeClose(BOOL * Cancel)
	{
		static BYTE parms[] = VTS_PBOOL ;
		InvokeHelper(0x60a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cancel);
	}
	void BeforeSave(BOOL SaveAsUI, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_BOOL VTS_PBOOL ;
		InvokeHelper(0x60b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SaveAsUI, Cancel);
	}
	void BeforePrint(BOOL * Cancel)
	{
		static BYTE parms[] = VTS_PBOOL ;
		InvokeHelper(0x60d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cancel);
	}
	void NewSheet(LPDISPATCH Sh)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x60e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh);
	}
	void AddinInstall()
	{
		InvokeHelper(0x610, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddinUninstall()
	{
		InvokeHelper(0x611, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void WindowResize(LPDISPATCH Wn)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x612, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Wn);
	}
	void WindowActivate(LPDISPATCH Wn)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x614, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Wn);
	}
	void WindowDeactivate(LPDISPATCH Wn)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x615, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Wn);
	}
	void SheetSelectionChange(LPDISPATCH Sh, LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0x616, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target);
	}
	void SheetBeforeDoubleClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_PBOOL ;
		InvokeHelper(0x617, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target, Cancel);
	}
	void SheetBeforeRightClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_PBOOL ;
		InvokeHelper(0x618, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target, Cancel);
	}
	void SheetActivate(LPDISPATCH Sh)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x619, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh);
	}
	void SheetDeactivate(LPDISPATCH Sh)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x61a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh);
	}
	void SheetCalculate(LPDISPATCH Sh)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x61b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh);
	}
	void SheetChange(LPDISPATCH Sh, LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0x61c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target);
	}
	void SheetFollowHyperlink(LPDISPATCH Sh, LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0x73e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target);
	}
	void SheetPivotTableUpdate(LPDISPATCH Sh, LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0x86d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target);
	}
	void PivotTableCloseConnection(LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x86e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Target);
	}
	void PivotTableOpenConnection(LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x86f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Target);
	}
	void Sync(long SyncEventType)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8da, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SyncEventType);
	}
	void BeforeXmlImport(LPDISPATCH Map, LPCTSTR Url, BOOL IsRefresh, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_BSTR VTS_BOOL VTS_PBOOL ;
		InvokeHelper(0x8eb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Map, Url, IsRefresh, Cancel);
	}
	void AfterXmlImport(LPDISPATCH Map, BOOL IsRefresh, long Result)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_BOOL VTS_I4 ;
		InvokeHelper(0x8ed, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Map, IsRefresh, Result);
	}
	void BeforeXmlExport(LPDISPATCH Map, LPCTSTR Url, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_BSTR VTS_PBOOL ;
		InvokeHelper(0x8ef, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Map, Url, Cancel);
	}
	void AfterXmlExport(LPDISPATCH Map, LPCTSTR Url, long Result)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_BSTR VTS_I4 ;
		InvokeHelper(0x8f0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Map, Url, Result);
	}
	void RowsetComplete(LPCTSTR Description, LPCTSTR Sheet, BOOL Success)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xa32, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Description, Sheet, Success);
	}
	void SheetPivotTableAfterValueChange(LPDISPATCH Sh, LPDISPATCH TargetPivotTable, LPDISPATCH TargetRange)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0xb4f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, TargetPivotTable, TargetRange);
	}
	void SheetPivotTableBeforeAllocateChanges(LPDISPATCH Sh, LPDISPATCH TargetPivotTable, long ValueChangeStart, long ValueChangeEnd, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xb50, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, TargetPivotTable, ValueChangeStart, ValueChangeEnd, Cancel);
	}
	void SheetPivotTableBeforeCommitChanges(LPDISPATCH Sh, LPDISPATCH TargetPivotTable, long ValueChangeStart, long ValueChangeEnd, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xb51, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, TargetPivotTable, ValueChangeStart, ValueChangeEnd, Cancel);
	}
	void SheetPivotTableBeforeDiscardChanges(LPDISPATCH Sh, LPDISPATCH TargetPivotTable, long ValueChangeStart, long ValueChangeEnd)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_I4 ;
		InvokeHelper(0xb52, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, TargetPivotTable, ValueChangeStart, ValueChangeEnd);
	}
	void SheetPivotTableChangeSync(LPDISPATCH Sh, LPDISPATCH Target)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0xb53, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Sh, Target);
	}
	void AfterSave(BOOL Success)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xb54, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Success);
	}
	void NewChart(LPDISPATCH Ch)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xb55, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Ch);
	}

	// WorkbookEvents プロパティ
public:

};

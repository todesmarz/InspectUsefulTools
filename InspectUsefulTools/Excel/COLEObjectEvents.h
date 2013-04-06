// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// COLEObjectEvents ラッパー クラス

class COLEObjectEvents : public COleDispatchDriver
{
public:
	COLEObjectEvents(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	COLEObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COLEObjectEvents(const COLEObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// OLEObjectEvents メソッド
public:
	void GotFocus()
	{
		InvokeHelper(0x605, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LostFocus()
	{
		InvokeHelper(0x606, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// OLEObjectEvents プロパティ
public:

};

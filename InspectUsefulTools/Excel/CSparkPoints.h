// [Typelib クラス追加ウィザード] で作成された IDispatch ラッパー クラスを生成しました。

//#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSparkPoints ラッパー クラス

class CSparkPoints : public COleDispatchDriver
{
public:
	CSparkPoints(){} // COleDispatchDriver 既定のコンストラクタを呼び出します。
	CSparkPoints(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSparkPoints(const CSparkPoints& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SparkPoints メソッド
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
	LPDISPATCH get_Negative()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Markers()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Highpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Lowpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Firstpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Lastpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb90, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// SparkPoints プロパティ
public:

};

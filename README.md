[reference::IReference](https://github.com/MasyoLab/Reference/blob/master/Reference.h)<BR>
	
```cpp
namespace reference {
	template <typename _Ty> class IReference;
}
```

# 概要
IReferenceは、[std::weak_ptr](https://cpprefjp.github.io/reference/memory/weak_ptr.html) を継承したクラスである。
### lock()の省略
std::weak_ptr では lock() を毎回書かなければオブジェクトのリソースにアクセスできないので、operator-> 内部で lock() をして取り出せるようにしたのが IReference です。

# メンバ関数
| 名前 | 説明 |
| ------ | ------ |
| (constructor) | コンストラクタ |
| (destructor) | デストラクタ | 
| operator bool |有効なリソースを所有しているかを判定する|
| operator!= |互いのリソースが違うかどうかの比較|
| operator== |互いのリソースが同じかどうかの比較|
| operator-> |メンバアクセス|
| operator* |メンバアクセス|
| operator= |代入演算子|
| check |監視対象が存在するかを判定する|
| clear |監視対象の監視をやめます|
| use_count |監視しているshared_ptrオブジェクトの所有者数を取得する|

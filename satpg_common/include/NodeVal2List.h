#ifndef NODEVAL2LIST_H
#define NODEVAL2LIST_H

/// @file NodeVal2List.h
/// @brief NodeVal2List のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class NodeVal NodeVal2List.h "NodeVal2List.h"
/// @brief ノードに対する値の割当を表すクラス
///
/// 昔の C でよく使われていたポインタの下位ビットが0であることを
/// 利用して，そこにフラグを埋め込むテクニック
/// C++ の時代では醜いことこのうえない．
//////////////////////////////////////////////////////////////////////
class NodeVal2
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は不定
  NodeVal2();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] time 時刻(0 or 1)
  /// @param[in] val 値
  NodeVal2(const TpgNode* node,
	   int time,
	   bool val);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  const TpgNode*
  node() const;

  /// @brief 時刻を返す．
  int
  time() const;

  /// @brief 値を返す．
  bool
  val() const;

  /// @brief 大小関係の比較関数
  friend
  bool
  operator<(const NodeVal2& left,
	    const NodeVal2& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パックした値
  ympuint mPackVal;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeVal2List NodeVal2List.h "NodeVal2List.h"
/// @brief ノードに対する値の割当を記録するクラス
//////////////////////////////////////////////////////////////////////
class NodeVal2List
{
public:

  /// @brief コンストラクタ
  ///
  /// 空のリストが生成される．
  NodeVal2List();

  /// @brief デストラクタ
  ~NodeVal2List();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 値を追加する．
  /// @param[in] node ノード
  /// @param[in] time 時刻(0 or 1)
  /// @param[in] val 値
  void
  add(const TpgNode* node,
      int time,
      bool val);

  /// @brief ソートする．
  void
  sort();

  /// @brief マージする．
  /// @param[in] src_list マージするリスト
  ///
  /// 矛盾する割当があった場合の動作は不定
  void
  merge(const NodeVal2List& src_list);

  /// @brief 差分を計算する．
  /// @param[in] src_list 差分の対象のリスト
  void
  diff(const NodeVal2List& src_list);

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  NodeVal2
  operator[](ymuint pos) const;

  /// @brief 矛盾した内容になっていないかチェックする．
  /// @return 正しければ true を返す．
  ///
  /// 具体的には同じノードで異なる値がある場合にエラーとなる．
  /// この関数はソートされている前提で動作する．
  bool
  sanity_check() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値割り当てのリスト
  vector<NodeVal2> mAsList;

};

/// @brief 2つの割当リストが矛盾しているか調べる．
bool
check_conflict(const NodeVal2List& src_list1,
	       const NodeVal2List& src_list2);

/// @brief 包含関係を調べる．
bool
check_contain(const NodeVal2List& src_list1,
	      const NodeVal2List& src_list2);

/// @brief 割当リストの内容を出力する．
ostream&
operator<<(ostream& s,
	   const NodeVal2List& src_list);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
//
// 内容は不定
inline
NodeVal2::NodeVal2() :
  mPackVal(0UL)
{
}

// @brief 値を指定したコンストラクタ
// @param[in] node ノード
// @param[in] time 時刻(0 or 1)
// @param[in] val 値
inline
NodeVal2::NodeVal2(const TpgNode* node,
		   int time,
		   bool val) :
  mPackVal(reinterpret_cast<ympuint>(node) | (time << 1) | val)
{
}

// @brief ノードを返す．
inline
const TpgNode*
NodeVal2::node() const
{
  return reinterpret_cast<const TpgNode*>(mPackVal & ~1UL);
}

// @brief 時刻を返す．
inline
int
NodeVal2::time() const
{
  return static_cast<int>((mPackVal >> 1) & 1U);
}

// @brief 値を返す．
inline
bool
NodeVal2::val() const
{
  return static_cast<bool>(mPackVal & 1UL);
}

// @brief 大小関係の比較関数
inline
bool
operator<(const NodeVal2& left,
	  const NodeVal2& right)
{
  return left.mPackVal < right.mPackVal;
}

// @brief 大小関係の比較関数
inline
bool
operator>(const NodeVal2& left,
	  const NodeVal2& right)
{
  return operator<(right, left);
}

// @brief 大小関係の比較関数
inline
bool
operator<=(const NodeVal2& left,
	   const NodeVal2& right)
{
  return !operator<(right, left);
}

// @brief 大小関係の比較関数
inline
bool
operator>=(const NodeVal2& left,
	   const NodeVal2& right)
{
  return !operator<(left, right);
}

// @brief コンストラクタ
inline
NodeVal2List::NodeVal2List()
{
}

// @brief デストラクタ
inline
NodeVal2List::~NodeVal2List()
{
}

// @brief 内容を空にする．
inline
void
NodeVal2List::clear()
{
  mAsList.clear();
}

// @brief 要素数を返す．
inline
ymuint
NodeVal2List::size() const
{
  return mAsList.size();
}

// @brief ソートする．
inline
void
NodeVal2List::sort()
{
  std::sort(mAsList.begin(), mAsList.end());
}

// @brief 値を追加する．
// @param[in] node ノード
// @param[in] time 時刻(0 or 1)
// @param[in] val 値
inline
void
NodeVal2List::add(const TpgNode* node,
		   int time,
		   bool val)
{
  mAsList.push_back(NodeVal2(node, time, val));
}

// @brief 要素を返す．
// @param[in] pos 位置 ( 0 <= pos < size() )
inline
NodeVal2
NodeVal2List::operator[](ymuint pos) const
{
  ASSERT_COND( pos < size() );
  return mAsList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // NODEVAL2LIST_H

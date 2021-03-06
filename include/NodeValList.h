#ifndef NODEVALLIST_H
#define NODEVALLIST_H

/// @file NodeValList.h
/// @brief NodeValList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class NodeVal NodeValList.h "NodeValList.h"
/// @brief ノードに対する値の割当を表すクラス
///
/// 昔の C でよく使われていたポインタの下位ビットが0であることを
/// 利用して，そこにフラグを埋め込むテクニック
/// C++ の時代では醜いことこのうえない．
//////////////////////////////////////////////////////////////////////
class NodeVal
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は不定
  NodeVal();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] val 値
  NodeVal(const TpgNode* node,
	  bool val);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  const TpgNode*
  node() const;

  /// @brief 値を返す．
  bool
  val() const;

  /// @brief 大小関係の比較関数
  friend
  bool
  operator<(const NodeVal& left,
	    const NodeVal& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パックした値
  ympuint mPackVal;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeValList NodeValList.h "NodeValList.h"
/// @brief ノードに対する値の割当を記録するクラス
//////////////////////////////////////////////////////////////////////
class NodeValList
{
public:

  /// @brief コンストラクタ
  ///
  /// 空のリストが生成される．
  NodeValList();

  /// @brief デストラクタ
  ~NodeValList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 値を追加する．
  /// @param[in] node ノード
  /// @param[in] val 値
  void
  add(const TpgNode* node,
      bool val);

  /// @brief ソートする．
  void
  sort();

  /// @brief マージする．
  /// @param[in] src_list マージするリスト
  ///
  /// 矛盾する割当があった場合の動作は不定
  void
  merge(const NodeValList& src_list);

  /// @brief 差分を計算する．
  /// @param[in] src_list 差分の対象のリスト
  void
  diff(const NodeValList& src_list);

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  NodeVal
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
  vector<NodeVal> mAsList;

};

/// @brief 2つの割当リストが矛盾しているか調べる．
bool
check_conflict(const NodeValList& src_list1,
	       const NodeValList& src_list2);

/// @brief 包含関係を調べる．
bool
check_contain(const NodeValList& src_list1,
	      const NodeValList& src_list2);

/// @brief 割当リストの内容を出力する．
ostream&
operator<<(ostream& s,
	   const NodeValList& src_list);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
//
// 内容は不定
inline
NodeVal::NodeVal() :
  mPackVal(0UL)
{
}

// @brief 値を指定したコンストラクタ
// @param[in] node ノード
// @param[in] val 値
inline
NodeVal::NodeVal(const TpgNode* node,
		 bool val) :
  mPackVal(reinterpret_cast<ympuint>(node) | val)
{
}

// @brief ノードを返す．
inline
const TpgNode*
NodeVal::node() const
{
  return reinterpret_cast<const TpgNode*>(mPackVal & ~1UL);
}

// @brief 値を返す．
inline
bool
NodeVal::val() const
{
  return static_cast<bool>(mPackVal & 1UL);
}

// @brief 大小関係の比較関数
inline
bool
operator<(const NodeVal& left,
	  const NodeVal& right)
{
  return left.mPackVal < right.mPackVal;
}

// @brief 大小関係の比較関数
inline
bool
operator>(const NodeVal& left,
	  const NodeVal& right)
{
  return operator<(right, left);
}

// @brief 大小関係の比較関数
inline
bool
operator<=(const NodeVal& left,
	   const NodeVal& right)
{
  return !operator<(right, left);
}

// @brief 大小関係の比較関数
inline
bool
operator>=(const NodeVal& left,
	   const NodeVal& right)
{
  return !operator<(left, right);
}

// @brief コンストラクタ
inline
NodeValList::NodeValList()
{
}

// @brief デストラクタ
inline
NodeValList::~NodeValList()
{
}

// @brief 内容を空にする．
inline
void
NodeValList::clear()
{
  mAsList.clear();
}

// @brief 要素数を返す．
inline
ymuint
NodeValList::size() const
{
  return mAsList.size();
}

// @brief ソートする．
inline
void
NodeValList::sort()
{
  std::sort(mAsList.begin(), mAsList.end());
}

// @brief 値を追加する．
// @param[in] node ノード
// @param[in] val 値
inline
void
NodeValList::add(const TpgNode* node,
		 bool val)
{
  mAsList.push_back(NodeVal(node, val));
}

// @brief 要素を返す．
// @param[in] pos 位置 ( 0 <= pos < size() )
inline
NodeVal
NodeValList::operator[](ymuint pos) const
{
  ASSERT_COND( pos < size() );
  return mAsList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // NODEVALLIST_H

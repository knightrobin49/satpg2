#ifndef BT2JUSTBASE_H
#define BT2JUSTBASE_H

/// @file Bt2JustBase.h
/// @brief Bt2JustBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2Impl.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Bt2JustBase Bt2JustBase.h "Bt2JustBase.h"
/// @brief 必要なノードのみ正当化する BackTracer の基底クラス
//////////////////////////////////////////////////////////////////////
class Bt2JustBase :
  public Bt2Impl
{
public:

  /// @brief コンストラクタ
  Bt2JustBase();

  /// @brief デストラクタ
  virtual
  ~Bt2JustBase();


public:
  //////////////////////////////////////////////////////////////////////
  // Backtracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief justified マークをつけ，mJustifiedNodeList に加える．
  /// @param[in] node 対象のノード
  void
  set_justified(const TpgNode* node);

  /// @brief justified マークを読む．
  /// @param[in] node 対象のノード
  bool
  justified_mark(const TpgNode* node);

  /// @brief justified マークをつけ，mJustifiedNodeList に加える．
  /// @param[in] node 対象のノード
  void
  set_justified0(const TpgNode* node);

  /// @brief justified マークを読む．
  /// @param[in] node 対象のノード
  bool
  justified0_mark(const TpgNode* node);

  /// @brief justified マークを消す．
  void
  clear_justified();

  /// @brief clear_justified() 中で呼ばれるフック関数
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  clear_justified_hook(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 正当化されたノードのリスト
  vector<const TpgNode*> mJustifiedNodeList;

  // 正当化マークの配列
  // インデックスは TpgNode::id()
  vector<bool> mJustifiedMarkArray;

  // 正当化されたノードのリスト
  vector<const TpgNode*> mJustified0NodeList;

  // 正当化マークの配列
  // インデックスは TpgNode::id()
  vector<bool> mJustified0MarkArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief justified マークをつける．
// @param[in] node 対象のノード
inline
void
Bt2JustBase::set_justified(const TpgNode* node)
{
  ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
  mJustifiedMarkArray[node->id()] = true;
  mJustifiedNodeList.push_back(node);
}

// @brief justified マークを読む．
// @param[in] node 対象のノード
inline
bool
Bt2JustBase::justified_mark(const TpgNode* node)
{
  ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
  return mJustifiedMarkArray[node->id()];
}

// @brief justified マークをつける．
// @param[in] node 対象のノード
inline
void
Bt2JustBase::set_justified0(const TpgNode* node)
{
  ASSERT_COND( node->id() < mJustified0MarkArray.size() );
  mJustified0MarkArray[node->id()] = true;
  mJustified0NodeList.push_back(node);
}

// @brief justified マークを読む．
// @param[in] node 対象のノード
inline
bool
Bt2JustBase::justified0_mark(const TpgNode* node)
{
  ASSERT_COND( node->id() < mJustified0MarkArray.size() );
  return mJustified0MarkArray[node->id()];
}

END_NAMESPACE_YM_SATPG

#endif // BT2JUST1_H

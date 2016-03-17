#ifndef FOCONE_H
#define FOCONE_H

/// @file FoCone.h
/// @brief FoCone のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FoCone FoCone.h "FoCone.h"
/// @brief 故障箇所の TFO に印をつけるためのクラス
//////////////////////////////////////////////////////////////////////
class FoCone
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値
  /// @param[in] fnode 故障位置のノード
  FoCone(ymuint max_node_id,
	 const TpgNode* fnode);

  /// @brief デストラクタ
  ~FoCone();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_id() const;

  /// @brief ノードの数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  const TpgNode*
  node(ymuint pos) const;

  /// @brief 出力のノードのリストを返す．
  const vector<const TpgNode*>&
  output_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マークを読む．
  /// @param[in] node 対象のノード
  bool
  mark(const TpgNode* node) const;

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_mark(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのIDの最大値
  ymuint mMaxNodeId;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<const TpgNode*> mNodeList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<const TpgNode*> mOutputList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値を返す．
inline
ymuint
FoCone::max_id() const
{
  return mMaxNodeId;
}

// @brief ノードの数を得る．
inline
ymuint
FoCone::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
inline
const TpgNode*
FoCone::node(ymuint pos) const
{
  return mNodeList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<const TpgNode*>&
FoCone::output_list() const
{
  return mOutputList;
}

// マークをつける．
inline
void
FoCone::set_mark(const TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
  mNodeList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }
}

// @brief tfo マークを読む．
inline
bool
FoCone::mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // FOCONE_H

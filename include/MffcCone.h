#ifndef MFFCCONE_H
#define MFFCCONE_H

/// @file MffcCone.h
/// @brief MffcCone のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "StructSat.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MffcCone MffcCone.h "MffcCone.h"
/// @brief MFFC 内の FFR の根に故障を挿入するためのクラス
//////////////////////////////////////////////////////////////////////
class MffcCone
{
public:

  /// @brief コンストラクタ
  /// @param[in] struct_sat StructSat ソルバ
  /// @param[in] fnode MFFC の根のノード
  MffcCone(StructSat& struct_sat,
	   const TpgNode* fnode);

  /// @brief デストラクタ
  ~MffcCone();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_id() const;

  /// @brief MFFC の根のノードを返す．
  const TpgNode*
  mffc_root() const;

  /// @brief 故障挿入箇所数を得る．
  ///
  /// 具体的には mffc_root()->mffc_elem_num() + 1
  /// を返す．
  ymuint
  mffc_elem_num() const;

  /// @brief 故障挿入箇所を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
  const TpgNode*
  mffc_elem(ymuint pos) const;

  /// @brief 出力のノードのリストを返す．
  const vector<const TpgNode*>&
  output_list() const;

  /// @brief 正常回路の変数マップを得る．
  const VidMap&
  gvar_map() const;

  /// @brief 故障回路の変数マップを得る．
  const VidMap&
  fvar_map() const;

  /// @brief 伝搬条件の変数マップを得る．
  const VidMap&
  dvar_map() const;

  /// @brief 正常値の変数を得る．
  SatVarId
  gvar(const TpgNode* node) const;

  /// @brief 故障値の変数を得る．
  SatVarId
  fvar(const TpgNode* node) const;

  /// @brief 伝搬値の変数を得る．
  SatVarId
  dvar(const TpgNode* node) const;

  /// @brief 故障挿入用の変数を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
  SatVarId
  mffc_elem_var(ymuint pos) const;

  /// @brief 故障挿入位置を選ぶ．
  /// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
  /// @param[out] assumptions 結果の割り当てを追加するベクタ
  void
  select_fault_node(ymuint pos,
		    vector<SatLiteral>& assumptions) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに故障値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] fvar 故障値の変数番号
  void
  set_fvar(const TpgNode* node,
	   SatVarId fvar);

  /// @brief ノードに伝搬値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] dvar 伝搬値の変数番号
  void
  set_dvar(const TpgNode* node,
	   SatVarId dvar);

  /// @brief 正常値と故障値が異なるという制約を追加する．
  /// @param[in] node 対象のノード
  void
  add_diff_clause(const TpgNode* node);

  /// @brief 故障伝搬条件を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] dst_node 伝搬条件の終点のノード
  void
  make_dchain_cnf(const TpgNode* node,
		  const TpgNode* dst_node);

  /// @brief マークを読む．
  /// @param[in] node 対象のノード
  bool
  mark(const TpgNode* node) const;

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_mark(const TpgNode* node);

  /// @brief SAT ソルバを得る．
  SatSolver&
  solver();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // struct SAT ソルバ
  StructSat& mStructSat;

  // ノードのIDの最大値
  ymuint mMaxNodeId;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障挿入位置のリスト
  vector<const TpgNode*> mElemList;

  // 故障回路の要素を納めたリスト
  vector<const TpgNode*> mNodeList;

  // mNodeList のうち故障位置の TFO の部分の数
  ymuint mTfoNum;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<const TpgNode*> mOutputList;

  // 故障値の変数マップ
  GenVidMap mFvarMap;

  // 故障伝搬値の変数マップ
  GenVidMap mDvarMap;

  // 故障挿入用の変数のリスト
  vector<SatVarId> mElemVarList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値を返す．
inline
ymuint
MffcCone::max_id() const
{
  return mMaxNodeId;
}

// @brief MFFC の根のノードを返す．
inline
const TpgNode*
MffcCone::mffc_root() const
{
  return mElemList[0];
}

// @brief 故障挿入箇所数を得る．
//
// 具体的には mffc_root()->mffc_elem_num() + 1
// を返す．
inline
ymuint
MffcCone::mffc_elem_num() const
{
  return mElemList.size();
}

// @brief 故障挿入箇所を得る．
// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
inline
const TpgNode*
MffcCone::mffc_elem(ymuint pos) const
{
  ASSERT_COND( pos < mffc_elem_num() );
  return mElemList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<const TpgNode*>&
MffcCone::output_list() const
{
  return mOutputList;
}

// @brief 正常回路の変数マップを得る．
inline
const VidMap&
MffcCone::gvar_map() const
{
  return mStructSat.var_map();
}

// @brief 故障回路の変数マップを得る．
inline
const VidMap&
MffcCone::fvar_map() const
{
  return mFvarMap;
}

// @brief 伝搬条件の変数マップを得る．
inline
const VidMap&
MffcCone::dvar_map() const
{
  return mDvarMap;
}

// @brief 正常値の変数を得る．
inline
SatVarId
MffcCone::gvar(const TpgNode* node) const
{
  return mStructSat.var(node);
}

// @brief 故障値の変数を得る．
inline
SatVarId
MffcCone::fvar(const TpgNode* node) const
{
  return mFvarMap(node);
}

// @brief 伝搬値の変数を得る．
inline
SatVarId
MffcCone::dvar(const TpgNode* node) const
{
  return mDvarMap(node);
}

// @brief 故障挿入用の変数を得る．
// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
inline
SatVarId
MffcCone::mffc_elem_var(ymuint pos) const
{
  ASSERT_COND( pos < mffc_elem_num() );
  return mElemVarList[pos];
}

// @brief ノードに故障値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] fvar 故障値の変数番号
inline
void
MffcCone::set_fvar(const TpgNode* node,
		     SatVarId fvar)
{
  mFvarMap.set_vid(node, fvar);
}

// @brief ノードに伝搬値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] dvar 伝搬値の変数番号
inline
void
MffcCone::set_dvar(const TpgNode* node,
		     SatVarId dvar)
{
  mDvarMap.set_vid(node, dvar);
}

// マークをつける．
inline
void
MffcCone::set_mark(const TpgNode* node)
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
MffcCone::mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// @brief SAT ソルバを得る．
inline
SatSolver&
MffcCone::solver()
{
  return mStructSat.solver();
}

END_NAMESPACE_YM_SATPG

#endif // MFFCCONE_H

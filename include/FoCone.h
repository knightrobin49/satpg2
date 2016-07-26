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
#include "StructSat.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FoCone FoCone.h "FoCone.h"
/// @brief 故障箇所の TFO に印をつけるためのクラス
//////////////////////////////////////////////////////////////////////
class FoCone
{
  friend class StructSat;

private:

  /// @brief コンストラクタ
  /// @param[in] struct_sat StructSat ソルバ
  /// @param[in] fnode 故障位置のノード
  /// @param[in] detect 検出条件
  FoCone(StructSat& struct_sat,
	 const TpgNode* fnode,
	 Val3 detect);

  /// @brief コンストラクタ
  /// @param[in] struct_sat StructSat ソルバ
  /// @param[in] fault 故障
  /// @param[in] detect 検出条件
  FoCone(StructSat& struct_sat,
	 const TpgFault* fault,
	 Val3 detect);

  /// @brief デストラクタ
  ~FoCone();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_id() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief fnode のファンアウトコーンの情報をセットする．
  /// @param[in] fnode 故障位置のノード
  /// @param[in] detect 検出条件
  void
  set(const TpgNode* fnode,
      Val3 detect);

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

  // 故障の TFO のノードリスト
  vector<const TpgNode*> mNodeList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<const TpgNode*> mOutputList;

  // 故障値の変数マップ
  GenVidMap mFvarMap;

  // 故障伝搬値の変数マップ
  GenVidMap mDvarMap;

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

// @brief 出力のノードのリストを返す．
inline
const vector<const TpgNode*>&
FoCone::output_list() const
{
  return mOutputList;
}

// @brief 正常回路の変数マップを得る．
inline
const VidMap&
FoCone::gvar_map() const
{
  return mStructSat.var_map();
}

// @brief 故障回路の変数マップを得る．
inline
const VidMap&
FoCone::fvar_map() const
{
  return mFvarMap;
}

// @brief 伝搬条件の変数マップを得る．
inline
const VidMap&
FoCone::dvar_map() const
{
  return mDvarMap;
}

// @brief 正常値の変数を得る．
inline
SatVarId
FoCone::gvar(const TpgNode* node) const
{
  return mStructSat.var(node);
}

// @brief 故障値の変数を得る．
inline
SatVarId
FoCone::fvar(const TpgNode* node) const
{
  return mFvarMap(node);
}

// @brief 伝搬値の変数を得る．
inline
SatVarId
FoCone::dvar(const TpgNode* node) const
{
  return mDvarMap(node);
}

// @brief ノードに故障値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] fvar 故障値の変数番号
inline
void
FoCone::set_fvar(const TpgNode* node,
		 SatVarId fvar)
{
  mFvarMap.set_vid(node, fvar);
}

// @brief ノードに伝搬値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] dvar 伝搬値の変数番号
inline
void
FoCone::set_dvar(const TpgNode* node,
		 SatVarId dvar)
{
  mDvarMap.set_vid(node, dvar);
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

// @brief SAT ソルバを得る．
inline
SatSolver&
FoCone::solver()
{
  return mStructSat.solver();
}

END_NAMESPACE_YM_SATPG

#endif // FOCONE_H

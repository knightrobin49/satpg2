#ifndef FVALCNFBASE_H
#define FVALCNFBASE_H

/// @file FvalCnfBase.h
/// @brief FvalCnfBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "GvalCnf.h"
#include "TpgNode.h"
#include "GenVidMap.h"
#include "Val3.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FvalCnfBase FvalCnfBase.h "FvalCnfBase.h"
/// @brief 故障回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class FvalCnfBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] gval_cnf 正常回路のCNFを作るクラス
  FvalCnfBase(GvalCnf& gval_cnf);

  /// @brief デストラクタ
  ~FvalCnfBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief 正常回路のCNFを生成するクラスを返す．
  GvalCnf&
  gval_cnf();

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

  /// @brief 故障検出用の変数番号を返す．
  SatVarId
  fd_var() const;

  /// @brief ノードに正常値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] gvar 正常値の変数番号
  void
  set_gvar(const TpgNode* node,
	   SatVarId gvar);

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

  /// @brief SATソルバを返す．
  SatSolver&
  solver();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver& mSolver;

  // 正常回路のCNFを作るクラス
  GvalCnf& mGvalCnf;

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
FvalCnfBase::max_node_id() const
{
  return mGvalCnf.max_node_id();
}

// @brief 正常回路のCNFを生成するクラスを返す．
inline
GvalCnf&
FvalCnfBase::gval_cnf()
{
  return mGvalCnf;
}

// @brief 正常回路の変数マップを得る．
inline
const VidMap&
FvalCnfBase::gvar_map() const
{
  return mGvalCnf.var_map();
}

// @brief 故障回路の変数マップを得る．
inline
const VidMap&
FvalCnfBase::fvar_map() const
{
  return mFvarMap;
}

// @brief 伝搬条件の変数マップを得る．
inline
const VidMap&
FvalCnfBase::dvar_map() const
{
  return mDvarMap;
}

// @brief 正常値の変数を得る．
inline
SatVarId
FvalCnfBase::gvar(const TpgNode* node) const
{
  return mGvalCnf.var(node);
}

// @brief 故障値の変数を得る．
inline
SatVarId
FvalCnfBase::fvar(const TpgNode* node) const
{
  return mFvarMap(node);
}

// @brief 伝搬値の変数を得る．
inline
SatVarId
FvalCnfBase::dvar(const TpgNode* node) const
{
  return mDvarMap(node);
}

// @brief ノードに正常値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] gvar 正常値の変数番号
inline
void
FvalCnfBase::set_gvar(const TpgNode* node,
		      SatVarId gvar)
{
  gval_cnf().set_var(node, gvar);
}

// @brief ノードに故障値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] fvar 故障値の変数番号
inline
void
FvalCnfBase::set_fvar(const TpgNode* node,
		      SatVarId fvar)
{
  mFvarMap.set_vid(node, fvar);
}

// @brief ノードに伝搬値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] dvar 伝搬値の変数番号
inline
void
FvalCnfBase::set_dvar(const TpgNode* node,
		      SatVarId dvar)
{
  mDvarMap.set_vid(node, dvar);
}

// @brief SATソルバを返す．
inline
SatSolver&
FvalCnfBase::solver()
{
  return mSolver;
}

END_NAMESPACE_YM_SATPG

#endif // FVALCNFBASE_H

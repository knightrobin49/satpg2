
/// @file FvalCnfBase.cc
/// @brief FvalCnfBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnfBase.h"
#include "GvalCnf.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeValList.h"
#include "ModelValMap.h"
#include "Extractor.h"
#include "BackTracer.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FvalCnfBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] gval_cnf 正常回路のCNFを作るクラス
FvalCnfBase::FvalCnfBase(GvalCnf& gval_cnf) :
  mSolver(gval_cnf.solver()),
  mGvalCnf(gval_cnf),
  mFvarMap(max_node_id()),
  mDvarMap(max_node_id())
{
}

// @brief デストラクタ
FvalCnfBase::~FvalCnfBase()
{
}

#if 0
// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[in] node_set 故障に関連するノード集合
// @param[out] suf_list 十分割当リストを格納する変数
void
FvalCnfBase::get_suf_list(const vector<SatBool3>& sat_model,
			  const TpgFault* fault,
			  const NodeSet& node_set,
			  NodeValList& suf_list)
{
  NodeValList dummy;
  get_pi_suf_list(sat_model, fault, node_set, suf_list, dummy);
}

// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[in] node_set 故障に関連するノード集合
// @param[out] suf_list 十分割当リストを格納する変数
// @param[out] pi_suf_list 外部入力上の十分割当リストを格納する変数
void
FvalCnfBase::get_pi_suf_list(const vector<SatBool3>& sat_model,
			     const TpgFault* fault,
			     const NodeSet& node_set,
			     NodeValList& suf_list,
			     NodeValList& pi_suf_list)
{
  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);

  Extractor extractor(val_map);
  extractor(fault, suf_list);

  BackTracer backtracer(max_node_id());
  backtracer(fault->tpg_node(), node_set, val_map, pi_suf_list);
}
#endif

// @brief 正常値と故障値が異なるという制約を追加する．
// @param[in] node 対象のノード
void
FvalCnfBase::add_diff_clause(const TpgNode* node)
{
  SatLiteral lit1(gvar(node));
  SatLiteral lit2(fvar(node));
  mSolver.add_clause( lit1,  lit2);
  mSolver.add_clause(~lit1, ~lit2);
}

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] dst_node 伝搬条件の終点のノード
void
FvalCnfBase::make_dchain_cnf(const TpgNode* node,
			     const TpgNode* dst_node)
{
  SatLiteral glit(gvar(node), false);
  SatLiteral flit(fvar(node), false);
  SatLiteral dlit(dvar(node), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  mSolver.add_clause(~glit, ~flit, ~dlit);
  mSolver.add_clause( glit,  flit, ~dlit);

  if ( node->is_output() || node == dst_node ) {
    // 出力ノードの場合，XOR(glit, flit) -> dlit となる．
    mSolver.add_clause(~glit,  flit, dlit);
    mSolver.add_clause( glit, ~flit, dlit);
  }
  else {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(nfo + 1);
    tmp_lits.push_back(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      const TpgNode* onode = node->active_fanout(j);
      SatLiteral odlit(dvar(onode), false);
      tmp_lits.push_back(odlit);
    }
    mSolver.add_clause(tmp_lits);
  }
}

END_NAMESPACE_YM_SATPG


/// @file FvalCnf.cc
/// @brief FvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnf.h"
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
// クラス FvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] gval_cnf 正常回路のCNFを作るクラス
FvalCnf::FvalCnf(GvalCnf& gval_cnf) :
  FvalCnfBase(gval_cnf)
{
}

// @brief デストラクタ
FvalCnf::~FvalCnf()
{
}

// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[in] node_set 故障に関連するノード集合
// @param[out] suf_list 十分割当リストを格納する変数
void
FvalCnf::get_suf_list(const vector<SatBool3>& sat_model,
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
FvalCnf::get_pi_suf_list(const vector<SatBool3>& sat_model,
			 const TpgFault* fault,
			 const NodeSet& node_set,
			 NodeValList& suf_list,
			 NodeValList& pi_suf_list)
{
  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);

  Extractor extractor(val_map);
  extractor(fault, suf_list);

  BackTracer backtracer(max_node_id());
  backtracer(fault->tpg_onode(), node_set, val_map, pi_suf_list);
}

// @brief 故障回路のCNFを作る．
// @param[in] src_node 故障位置のノード
// @param[in] node_set 故障に関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
FvalCnf::make_cnf(const TpgNode* src_node,
		  const NodeSet& node_set,
		  Val3 detect)
{
  gval_cnf().make_cnf(node_set);

  const TpgNode* dom_node = node_set.dom_node();

  ymuint n = node_set.tfo_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId fvar = solver().new_var();
    SatVarId dvar = solver().new_var();
    set_fvar(node, fvar);
    set_dvar(node, dvar);
  }
  ymuint n0 = node_set.tfo_tfi_size();
  for (ymuint i = n; i < n0; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    set_fvar(node, gvar(node));
  }

  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( node != src_node ) {
      // 故障回路のゲートの入出力関係を表すCNFを作る．
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(node, dom_node);
  }

  const vector<const TpgNode*>& output_list = node_set.output_list();
  ymuint npo = output_list.size();

  if ( detect == kVal0 ) {
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      SatLiteral dlit(dvar(node));
      solver().add_clause(~dlit);
    }
    if ( dom_node != nullptr ) {
      SatLiteral dlit(dvar(dom_node));
      solver().add_clause(~dlit);
    }
  }
  else if ( detect == kVal1 ) {
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(npo + 1);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      SatLiteral dlit(dvar(node));
      tmp_lits.push_back(dlit);
    }
    if ( dom_node != nullptr ) {
      SatLiteral dlit(dvar(dom_node));
      tmp_lits.push_back(dlit);
    }
    solver().add_clause(tmp_lits);

    for (const TpgNode* node = src_node; node != nullptr && node != dom_node; node = node->imm_dom()) {
      SatLiteral dlit(dvar(node));
      solver().add_clause(dlit);
    }
  }
  else {
    SatVarId fdvar = solver().new_var();
    set_fdvar(fdvar);
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(npo + 1);
    SatLiteral fdlit(fdvar);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      SatLiteral dlit(dvar(node));
      tmp_lits.push_back(dlit);
      solver().add_clause(fdlit, ~dlit);
    }
    tmp_lits.push_back(~fdlit);
    solver().add_clause(tmp_lits);

    for (const TpgNode* node = src_node; node != nullptr && node != dom_node; node = node->imm_dom()) {
      SatLiteral dlit(dvar(node));
      solver().add_clause(~fdlit, dlit);
    }
  }
}

// @brief 故障回路のCNFを作る．
// @param[in] fault 故障
// @param[in] node_set 故障に関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
FvalCnf::make_cnf(const TpgFault* fault,
		  const NodeSet& node_set,
		  Val3 detect)
{
  make_cnf(fault->tpg_onode(), node_set, detect);

  NodeValList assign_list;
  gval_cnf().add_fault_condition(fault, assign_list);

  if ( detect == kVal0 ) {
    gval_cnf().add_negation(assign_list);
  }
  else if ( detect == kVal1 ) {
    gval_cnf().add_assignments(assign_list);
  }
  else {
    SatLiteral fdlit(fd_var(), false);
    ymuint n = assign_list.size();
    vector<SatLiteral> tmp_lits(n + 1);
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = assign_list[i];
      const TpgNode* node = nv.node();
      bool val = nv.val();
      SatLiteral clit(gvar(node), val);
      solver().add_clause(~fdlit, clit);
      tmp_lits[i] = ~clit;
    }
    tmp_lits[n] = fdlit;
  }
}

END_NAMESPACE_YM_SATPG


/// @file FvalCnf.cc
/// @brief FvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016 Yusuke Matsunaga
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
  mDebugFlag = 0;
}

// @brief デストラクタ
FvalCnf::~FvalCnf()
{
}

// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[out] suf_list 十分割当リストを格納する変数
void
FvalCnf::get_suf_list(const vector<SatBool3>& sat_model,
		      const TpgFault* fault,
		      NodeValList& suf_list)
{
  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);

  Extractor extractor(val_map);
  extractor(fault, suf_list);
  suf_list.sort();
}

// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[in] po_list 故障に関連する外部出力のリスト
// @param[out] suf_list 十分割当リストを格納する変数
// @param[out] pi_suf_list 外部入力上の十分割当リストを格納する変数
void
FvalCnf::get_pi_suf_list(const vector<SatBool3>& sat_model,
			 const TpgFault* fault,
			 const vector<const TpgNode*>& po_list,
			 NodeValList& suf_list,
			 NodeValList& pi_suf_list)
{
  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);

  Extractor extractor(val_map);
  extractor(fault, suf_list);
  suf_list.sort();

  BackTracer backtracer(max_node_id());
  backtracer(fault->tpg_onode(), po_list, val_map, pi_suf_list);
  pi_suf_list.sort();
}

// @brief 故障伝搬条件のCNFを作る．
// @param[in] src_node 故障位置のノード
// @param[in] node_set 故障に関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
FvalCnf::make_prop_cnf(const TpgNode* src_node,
		       const NodeSet& node_set,
		       Val3 detect)
{
  gval_cnf().make_cnf(node_set);

  const TpgNode* dom_node = node_set.dom_node();

  // fault cone に属する部分のノードに対応する変数を用意する．
  ymuint n = node_set.tfo_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId fvar = solver().new_var();
    set_fvar(node, fvar);
    if ( debug() ) {
      cout << "fvar(" << node->name() << ") = " << fvar << endl;
    }

    if ( detect == kVal1 ) {
      SatVarId dvar = solver().new_var();
      set_dvar(node, dvar);
    }
  }

  // それ以外の部分のノードに対応する fvar は gvar を使う．
  ymuint n0 = node_set.tfo_tfi_size();
  for (ymuint i = n; i < n0; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    set_fvar(node, gvar(node));
    if ( debug() ) {
      cout << "gvar(" << node->name() << ") = " << gvar(node) << endl;
      cout << "fvar(" << node->name() << ") = " << gvar(node) << endl;
    }
  }

  // 故障回路のゲートの入出力関係を表すCNFを作る．
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( node != src_node ) {
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }
  }

  const vector<const TpgNode*>& output_list = node_set.output_list();
  ymuint npo = output_list.size();

  if ( detect == kVal0 ) {
    // 故障を検出しない条件
    if ( dom_node == nullptr ) {
      // 全ての出力の dlit が 0
      for (ymuint i = 0; i < npo; ++ i) {
	const TpgNode* node = output_list[i];
	SatLiteral glit(gvar(node), false);
	SatLiteral flit(fvar(node), false);
	solver().add_clause(~glit,  flit);
	solver().add_clause( glit, ~flit);
      }
    }
    else {
      // dominator の dlit が 0
      SatLiteral glit(gvar(dom_node), false);
      SatLiteral flit(fvar(dom_node), false);
      solver().add_clause(~glit,  flit);
      solver().add_clause( glit, ~flit);
    }
  }
  else if ( detect == kVal1 ) {
    // 故障を検出する条件
    for (ymuint i = 0; i < n; ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);

      // D-Chain 制約を作る．
      make_dchain_cnf(node, dom_node);
    }
    if ( dom_node == nullptr ) {
      // 出力の最低1つの dlit が 1
      vector<SatLiteral> tmp_lits(npo);
      for (ymuint i = 0; i < npo; ++ i) {
	const TpgNode* node = output_list[i];
	SatLiteral dlit(dvar(node));
	tmp_lits[i] = dlit;
      }
      solver().add_clause(tmp_lits);
    }
    else {
      // dominator の dlit が 1
      SatLiteral dlit(dvar(dom_node));
      solver().add_clause(dlit);
    }
    // src_node の dlit が 1
    SatLiteral dlit(dvar(src_node));
    solver().add_clause(dlit);
  }
  else {
    // 変数に応じて検出かどうかをコントロールする．
    SatVarId fdvar = solver().new_var();
    set_fdvar(fdvar);
    SatLiteral fdlit(fdvar);
    if ( dom_node == nullptr ) {
      vector<SatLiteral> tmp_lits;
      tmp_lits.reserve(npo + 1);
      for (ymuint i = 0; i < npo; ++ i) {
	const TpgNode* node = output_list[i];
	SatLiteral dlit(dvar(node));
	tmp_lits.push_back(dlit);
	solver().add_clause(fdlit, ~dlit);
      }
      tmp_lits.push_back(~fdlit);
      solver().add_clause(tmp_lits);
    }
    else {
      SatLiteral dlit(dvar(dom_node));
      solver().add_clause(~fdlit,  dlit);
      solver().add_clause( fdlit, ~dlit);
    }
    SatLiteral dlit(dvar(src_node));
    solver().add_clause(~fdlit,  dlit);
    solver().add_clause( fdlit, ~dlit);
  }
}

// @brief ノードの fvar を gvar にする．
// @param[in] node ノード
// @param[in] mark 処理済みの印
void
FvalCnf::set_fvar_recur(const TpgNode* node,
			vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  set_fvar(node, gvar(node));
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    set_fvar_recur(inode, mark);
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
  // 故障の伝搬条件を作る．
  const TpgNode* src_node = fault->tpg_onode();
  make_prop_cnf(src_node, node_set, detect);

  if ( fault->is_branch_fault() ) {
    // ブランチの故障の場合はちょっと面倒
    // 故障のある入力を故障値に縮退した論理式を作る必要がある．
    src_node->make_faulty_cnf(solver(), fault->tpg_pos(), fault->val(), VidLitMap(src_node, fvar_map()));
  }
  else {
    // 出力の故障の場合は故障値をそのまま固定する．
    SatLiteral flit(fvar(src_node));
    if ( fault->val() == 0 ) {
      solver().add_clause(~flit);
    }
    else {
      solver().add_clause(flit);
    }
  }
}

END_NAMESPACE_YM_SATPG

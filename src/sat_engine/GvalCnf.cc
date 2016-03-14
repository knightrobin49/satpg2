
/// @file GvalCnf.cc
/// @brief GvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GvalCnf.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "TpgFault.h"
#include "TpgNode.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス GvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
GvalCnf::GvalCnf(ymuint max_node_id,
		 const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp) :
  mSolver(sat_type, sat_option, sat_outp),
  mMaxId(max_node_id),
  mMark(max_node_id, false),
  mVarMap(max_node_id)
{
}

// @brief デストラクタ
GvalCnf::~GvalCnf()
{
}

// @brief 故障の検出条件を割当リストに追加する．
// @param[in] fault 故障
// @param[out] assignment 割当リスト
void
GvalCnf::add_fault_condition(const TpgFault* fault,
			     NodeValList& assignment)
{
  // 故障の活性化条件
  const TpgNode* inode = fault->tpg_inode();
  // 0縮退故障の時 1にする．
  bool val = (fault->val() == 0);
  assignment.add(inode, val);

  if ( fault->is_branch_fault() ) {
    // 故障の伝搬条件
    const TpgNode* onode = fault->tpg_onode();
    Val3 nval = onode->nval();
    if ( nval != kValX ) {
      bool val = (nval == kVal1);
      // inode -> onode の伝搬条件
      ymuint ni = onode->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const TpgNode* inode1 = onode->fanin(i);
	if ( inode1 == inode ) {
	  continue;
	}
	assignment.add(inode1, val);
      }
    }
  }
}

// @brief FFR内の故障の伝搬条件を割当リストに追加する．
// @param[in] root_node FFRの根のノード
// @param[in] fault 故障
// @param[out] assignment 割当リスト
void
GvalCnf::add_ffr_condition(const TpgNode* root_node,
			   const TpgFault* fault,
			   NodeValList& assignment)
{
  add_fault_condition(fault, assignment);

  // 故障の伝搬条件
  for (const TpgNode* node = fault->tpg_onode(); node != root_node; node = node->active_fanout(0)) {
    ASSERT_COND( node->active_fanout_num() == 1 );
    const TpgNode* onode = node->active_fanout(0);
    Val3 nval = onode->nval();
    if ( nval == kValX ) {
      continue;
    }
    bool val = (nval == kVal1);
    ymuint ni = onode->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = onode->fanin(i);
      if ( inode == node ) {
	continue;
      }
      assignment.add(inode, val);
    }
  }
}

// @brief 割当リストに従って値を固定する．
// @param[in] assignment 割当リスト
void
GvalCnf::add_assignments(const NodeValList& assignment)
{
  ymuint n = assignment.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assignment[i];
    const TpgNode* node = nv.node();
    make_cnf(node);
    SatLiteral alit(var(node), false);
    if ( nv.val() ) {
      mSolver.add_clause(alit);
    }
    else {
      mSolver.add_clause(~alit);
    }
  }
}

// @brief 割当リストの否定の節を加える．
// @param[in] assignment 割当リスト
void
GvalCnf::add_negation(const NodeValList& assignment)
{
  ymuint n = assignment.size();
  vector<SatLiteral> tmp_lits(n);
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assignment[i];
    const TpgNode* node = nv.node();
    make_cnf(node);
    SatLiteral alit(var(node), false);
    if ( nv.val() ) {
      tmp_lits[i] = ~alit;
    }
    else {
      tmp_lits[i] = alit;
    }
  }
  mSolver.add_clause(tmp_lits);
}

// @brief 割当リストを仮定のリテラルに変換する．
// @param[in] assign_list 割当リスト
// @param[out] assumptions 仮定を表すリテラルのリスト
//
// 必要に応じて使われているリテラルに関するCNFを追加する．
void
GvalCnf::conv_to_assumption(const NodeValList& assign_list,
			    vector<SatLiteral>& assumptions)
{
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_cnf(node);
    SatLiteral alit(var(node), false);
    if ( nv.val() ) {
      assumptions.push_back(alit);
    }
    else {
      assumptions.push_back(~alit);
    }
  }
}

// @brief NodeSet に含まれるノードの CNF を作る．
// @param[in] node_set ノード集合
void
GvalCnf::make_cnf(const NodeSet& node_set)
{
  ymuint n = node_set.tfo_tfi_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( mark(node) ) {
      continue;
    }
    SatVarId gvar = mSolver.new_var();
    set_var(node, gvar);
  }
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( mark(node) ) {
      continue;
    }
    set_mark(node);
    node->make_cnf(mSolver, VidLitMap(node, var_map()));
  }
}

// @brief node の TFI の CNF を作る．
// @param[in] node 対象のノード
void
GvalCnf::make_cnf(const TpgNode* node)
{
  if ( mark(node) ) {
    return;
  }
  set_mark(node);

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_cnf(inode);
  }

  SatVarId gvar = mSolver.new_var();
  set_var(node, gvar);

  node->make_cnf(mSolver, VidLitMap(node, var_map()));
}

// @brief チェックを行う．
// @param[out] sat_model SATの場合の解
SatBool3
GvalCnf::check_sat(vector<SatBool3>& sat_model)
{
  return mSolver.solve(sat_model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] assign_list 割当リスト
// @param[out] sat_model SATの場合の解
SatBool3
GvalCnf::check_sat(const NodeValList& assign_list,
		   vector<SatBool3>& sat_model)
{
  vector<SatLiteral> assumptions;
  conv_to_assumption(assign_list, assumptions);

  return mSolver.solve(assumptions, sat_model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] assign_list1, assign_list2 割当リスト
// @param[out] sat_model SATの場合の解
SatBool3
GvalCnf::check_sat(const NodeValList& assign_list1,
		   const NodeValList& assign_list2,
		   vector<SatBool3>& sat_model)
{
  vector<SatLiteral> assumptions;

  conv_to_assumption(assign_list1, assumptions);
  conv_to_assumption(assign_list2, assumptions);

  return mSolver.solve(assumptions, sat_model);
}

END_NAMESPACE_YM_SATPG

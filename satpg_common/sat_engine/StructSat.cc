
/// @file StructSat.cc
/// @brief StructSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "StructSat.h"
#include "FoCone.h"
#include "MffcCone.h"
#include "NodeValList.h"
#include "TpgFault.h"
#include "TpgNode.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス StructSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
StructSat::StructSat(ymuint max_node_id,
		     const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp) :
  mSolver(sat_type, sat_option, sat_outp),
  mMaxId(max_node_id),
  mMark(max_node_id, false),
  mVarMap(max_node_id)
{
  mDebugFlag = 0;
}

// @brief デストラクタ
StructSat::~StructSat()
{
  for (ymuint i = 0; i < mFoConeList.size(); ++ i) {
    FoCone* cone = mFoConeList[i];
    delete cone;
  }
  for (ymuint i = 0; i < mMffcConeList.size(); ++ i) {
    MffcCone* cone = mMffcConeList[i];
    delete cone;
  }
}

// @brief fault cone を追加する．
// @param[in] fnode 故障のあるノード
// @param[in] detect 検出条件
const FoCone*
StructSat::add_focone(const TpgNode* fnode,
		      Val3 detect)
{
  FoCone* focone = new FoCone(*this, fnode, detect);
  mFoConeList.push_back(focone);
  return focone;
}

// @brief fault cone を追加する．
// @param[in] fnode 故障のあるノード
// @param[in] bnode ブロックノード
// @param[in] detect 検出条件
const FoCone*
StructSat::add_focone(const TpgNode* fnode,
		      const TpgNode* bnode,
		      Val3 detect)
{
  FoCone* focone = new FoCone(*this, fnode, bnode, detect);
  mFoConeList.push_back(focone);
  return focone;
}

// @brief fault cone を追加する．
// @param[in] fault 故障
// @param[in] detect 検出条件
const FoCone*
StructSat::add_focone(const TpgFault* fault,
		      Val3 detect)
{
  const TpgNode* fnode = fault->tpg_onode();
  FoCone* focone = new FoCone(*this, fnode, detect);
  mFoConeList.push_back(focone);

  if ( detect == kVal1 ) {
    NodeValList assignment;
    add_fault_condition(fault, assignment);
    add_assignments(assignment);
  }
  else {
    int fval = fault->val();
    if ( fault->is_branch_fault() ) {
      ymuint pos = fault->tpg_pos();
      fnode->make_faulty_cnf(solver(), pos, fval, VidLitMap(fnode, focone->fvar_map()));
    }
    else {
      SatLiteral flit(focone->fvar(fnode));
      if ( fval == 0 ) {
	solver().add_clause(~flit);
      }
      else {
	solver().add_clause(flit);
      }
    }
  }
  return focone;
}

// @brief fault cone を追加する．
// @param[in] fault 故障
// @param[in] bnode ブロックノード
// @param[in] detect 検出条件
const FoCone*
StructSat::add_focone(const TpgFault* fault,
		      const TpgNode* bnode,
		      Val3 detect)
{
  const TpgNode* fnode = fault->tpg_onode();
  FoCone* focone = new FoCone(*this, fnode, bnode, detect);
  mFoConeList.push_back(focone);

  if ( detect == kVal1 ) {
    NodeValList assignment;
    add_fault_condition(fault, assignment);
    add_assignments(assignment);
  }
  else {
    int fval = fault->val();
    if ( fault->is_branch_fault() ) {
      ymuint pos = fault->tpg_pos();
      fnode->make_faulty_cnf(solver(), pos, fval, VidLitMap(fnode, focone->fvar_map()));
    }
    else {
      SatLiteral flit(focone->fvar(fnode));
      if ( fval == 0 ) {
	solver().add_clause(~flit);
      }
      else {
	solver().add_clause(flit);
      }
    }
  }
  return focone;
}

// @brief fault cone を追加する．
// @param[in] fnode 故障のあるノード
//
// こちらは MFFC 内の故障を対象にする．
const MffcCone*
StructSat::add_mffccone(const TpgNode* fnode)
{
  MffcCone* mffc_cone = new MffcCone(*this, fnode);
  mMffcConeList.push_back(mffc_cone);
  return mffc_cone;
}

// @brief 故障の検出条件を割当リストに追加する．
// @param[in] fault 故障
// @param[out] assignment 割当リスト
void
StructSat::add_fault_condition(const TpgFault* fault,
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
StructSat::add_ffr_condition(const TpgNode* root_node,
			     const TpgFault* fault,
			     NodeValList& assignment)
{
  // ノードに対する故障の伝搬条件
  add_fault_condition(fault, assignment);

  // FFR の根までの伝搬条件
  for (const TpgNode* node = fault->tpg_onode(); node != root_node;
       node = node->active_fanout(0)) {
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
StructSat::add_assignments(const NodeValList& assignment)
{
  ymuint n = assignment.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assignment[i];
    const TpgNode* node = nv.node();
    // node およびその TFI に関する節を追加する．
    make_tfi_cnf(node);

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
StructSat::add_negation(const NodeValList& assignment)
{
  ymuint n = assignment.size();
  vector<SatLiteral> tmp_lits(n);
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assignment[i];
    const TpgNode* node = nv.node();
    // node およびその TFI に関する節を追加する．
    make_tfi_cnf(node);

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
StructSat::conv_to_assumption(const NodeValList& assign_list,
			      vector<SatLiteral>& assumptions)
{
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    // node およびその TFI に関する節を追加する．
    make_tfi_cnf(node);

    SatLiteral alit(var(node), false);
    if ( nv.val() ) {
      assumptions.push_back(alit);
    }
    else {
      assumptions.push_back(~alit);
    }
  }
}

// @brief node の TFI の CNF を作る．
// @param[in] node 対象のノード
void
StructSat::make_tfi_cnf(const TpgNode* node)
{
  if ( mark(node) ) {
    return;
  }
  set_mark(node);

  // 先に TFI のノードの節を作る．
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_tfi_cnf(inode);
  }

  // node に対応する変数を用意する．
  SatVarId gvar = mSolver.new_var();
  set_var(node, gvar);

  // node の入出力の関係を表す節を作る．
  node->make_cnf(mSolver, VidLitMap(node, var_map()));
}

// @brief node に関する故障伝搬条件を作る．
void
StructSat::make_dchain_cnf(const TpgNode* node,
			   const TpgNode* dst_node,
			   const VidMap& fvar_map,
			   const VidMap& dvar_map)
{
  SatLiteral glit(var(node), false);
  SatLiteral flit(fvar_map(node), false);
  SatLiteral dlit(dvar_map(node), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  solver().add_clause(~glit, ~flit, ~dlit);
  solver().add_clause( glit,  flit, ~dlit);

  if ( node->is_output() || node == dst_node ) {
    // 出力ノードの場合，XOR(glit, flit) -> dlit となる．
    solver().add_clause(~glit,  flit, dlit);
    solver().add_clause( glit, ~flit, dlit);
  }
  else {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(nfo + 1);
    tmp_lits.push_back(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      const TpgNode* onode = node->active_fanout(j);
      SatLiteral odlit(dvar_map(onode), false);
      tmp_lits.push_back(odlit);
    }
    solver().add_clause(tmp_lits);

    // immediate dominator がある場合，immediate dominator の dlit も 1 でなければならない．
    const TpgNode* idom = node->imm_dom();
    if ( idom != nullptr ) {
      SatLiteral odlit(dvar_map(idom));
      solver().add_clause(~dlit, odlit);
    }
  }
}

// @brief チェックを行う．
// @param[out] sat_model SATの場合の解
SatBool3
StructSat::check_sat(vector<SatBool3>& sat_model)
{
  return mSolver.solve(sat_model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] assign_list 割当リスト
// @param[out] sat_model SATの場合の解
SatBool3
StructSat::check_sat(const NodeValList& assign_list,
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
StructSat::check_sat(const NodeValList& assign_list1,
		     const NodeValList& assign_list2,
		     vector<SatBool3>& sat_model)
{
  vector<SatLiteral> assumptions;

  conv_to_assumption(assign_list1, assumptions);
  conv_to_assumption(assign_list2, assumptions);

  return mSolver.solve(assumptions, sat_model);
}

END_NAMESPACE_YM_SATPG

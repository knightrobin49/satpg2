
/// @file GvalCnf.cc
/// @brief GvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GvalCnf.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "SatEngine.h"
#include "TpgNode.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス GvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SAT ソルバ
// @param[in] max_node_id ノード番号の最大値
GvalCnf::GvalCnf(SatSolver& solver,
		 ymuint max_node_id) :
  mSolver(solver),
  mMaxId(max_node_id),
  mMark(max_node_id, false),
  mVarMap(max_node_id)
{
}

// @brief デストラクタ
GvalCnf::~GvalCnf()
{
}

// @brief 初期化する．
// @param[in] max_node_id ノード番号の最大値
void
GvalCnf::init(ymuint max_node_id)
{
  mMaxId = max_node_id;
  mMark.clear();
  mMark.resize(max_node_id, false);
  mVarMap.init(max_node_id);
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

// @brief 割当リストに対応する仮定を追加する．
// @param[in] assignment 割当リスト
// @param[out] assumptions 仮定を表すリテラルのリスト
void
GvalCnf::add_assumption(const NodeValList& assignment,
			vector<SatLiteral>& assumptions)
{
  ymuint n = assignment.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assignment[i];
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

END_NAMESPACE_YM_SATPG

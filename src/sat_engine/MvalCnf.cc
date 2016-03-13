
/// @file MvalCnf.cc
/// @brief MvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MvalCnf.h"
#include "NodeSet.h"
#include "TpgFault.h"
#include "VectLitMap.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] gval_cnf 正常回路のCNFを作るクラス
MvalCnf::MvalCnf(GvalCnf& gval_cnf) :
  FvalCnfBase(gval_cnf),
  mFdMapArray(max_node_id(), nullptr)
{
}

// @brief デストラクタ
MvalCnf::~MvalCnf()
{
  for (ymuint i = 0; i < max_node_id(); ++ i) {
    delete mFdMapArray[i];
  }
}

// @brief ノードに検出対象の故障があるか調べる．
bool
MvalCnf::has_faults(const TpgNode* node) const
{
  return mFdMapArray[node->id()] != nullptr;
}

// @brief 出力の故障に対応した変数を得る．
// @param[in] node ノード
// @param[in] fval 故障値 (0/1)
SatVarId
MvalCnf::ofvar(const TpgNode* node,
	       int fval) const
{
  ASSERT_COND( has_faults(node) );
  ASSERT_COND( fval == 0 || fval == 1 );
  return mFdMapArray[node->id()]->mOvar[fval];
}

// @brief 入力の故障に対応した変数を得る．
// @param[in] node ノード
// @param[in] pos 入力番号
// @param[in] fval 故障値 (0/1)
SatVarId
MvalCnf::ifvar(const TpgNode* node,
	       ymuint pos,
	       int fval) const
{
  ASSERT_COND( has_faults(node) );
  ASSERT_COND( pos < node->fanin_num() );
  ASSERT_COND( fval == 0 || fval == 1 );
  return mFdMapArray[node->id()]->mIvar[pos * 2 + fval];
}

// @brief 出力の故障検出用の変数を割り当てる．
// @param[in] node ノード
// @param[in] fval 故障値 (0/1)
// @param[in] fdvar 変数番号
void
MvalCnf::set_ofvar(const TpgNode* node,
		   int fval,
		   SatVarId fdvar)
{
  ASSERT_COND( fval == 0 || fval == 1 );
  FdMap* fdmap = setup_fdmap(node);
  fdmap->mOvar[fval] = fdvar;
}

// @brief 入力の故障検出用の変数を割り当てる．
// @param[in] node ノード
// @param[in] pos 入力番号
// @param[in] fval 故障値 (0/1)
// @param[in] fdvar 変数番号
void
MvalCnf::set_ifvar(const TpgNode* node,
		   ymuint pos,
		   int fval,
		   SatVarId fdvar)
{
  ASSERT_COND( pos < node->fanin_num() );
  ASSERT_COND( fval == 0 || fval == 1 );
  FdMap* fdmap = setup_fdmap(node);
  fdmap->mIvar[pos * 2 + fval] = fdvar;
}

// @brief FdMap を初期化する．
// @param[in] node ノード
MvalCnf::FdMap*
MvalCnf::setup_fdmap(const TpgNode* node)
{
  ymuint id = node->id();
  FdMap* fdmap = mFdMapArray[id];
  if ( fdmap == nullptr ) {
    ymuint ni = node->fanin_num();
    fdmap = new FdMap(ni);
    fdmap->mOvar[0] = kSatVarIdIllegal;
    fdmap->mOvar[1] = kSatVarIdIllegal;
    for (ymuint i = 0; i < ni; ++ i) {
      fdmap->mIvar[i * 2 + 0] = kSatVarIdIllegal;
      fdmap->mIvar[i * 2 + 1] = kSatVarIdIllegal;
    }
    mFdMapArray[id] = fdmap;
  }
  return fdmap;
}

// @brief 複数故障検出回路のCNFを作る．
// @param[in] fault_list 故障リスト
// @param[in] fnode_list 故障を持つノードのリスト
// @param[in] node_set 故障に関係するノード集合
void
MvalCnf::make_cnf(const vector<const TpgFault*>& fault_list,
		  const vector<const TpgNode*>& fnode_list,
		  const NodeSet& node_set)
{
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId gvar = solver().new_var();
    set_gvar(node, gvar);
    set_fvar(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId fvar = solver().new_var();
    SatVarId dvar = solver().new_var();
    set_fvar(node, fvar);
    set_dvar(node, dvar);
  }

  // 故障を活性化するとき true にする変数．
  ymuint nf = fault_list.size();
  set_fault_num(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SatVarId fdvar = solver().new_var();
    set_fault_var(i, fdvar);
    const TpgFault* f = fault_list[i];
    int fval = f->val();
    const TpgNode* node = f->tpg_node();

    if ( f->is_output_fault() ) {
      set_ofvar(node, fval, fdvar);
    }
    else {
      ymuint pos = f->tpg_pos();
      set_ifvar(node, pos, fval, fdvar);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    node->make_cnf(solver(), VidLitMap(node, gvar_map()));
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( has_faults(node) ) {
      make_fnode_cnf(node);
    }
    else {
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }

    make_dchain_cnf(node, node_set.dom_node());
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  vector<SatLiteral> tmp_lits(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    SatLiteral dlit(dvar(node));
    tmp_lits[i] = dlit;
  }
  solver().add_clause(tmp_lits);
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
MvalCnf::make_fnode_cnf(const TpgNode* node)
{
  ymuint ni = node->fanin_num();
  vector<SatVarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    SatVarId f0_var = ifvar(node, i, 0);
    SatVarId f1_var = ifvar(node, i, 1);
    if ( f0_var == kSatVarIdIllegal ) {
      if ( f1_var == kSatVarIdIllegal ) {
	ivars[i] = fvar(inode);
      }
      else {
	SatVarId tmp_var = solver().new_var();
	make_flt1_cnf(fvar(inode), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kSatVarIdIllegal ) {
	SatVarId tmp_var = solver().new_var();
	make_flt0_cnf(fvar(inode), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	SatVarId tmp_var = solver().new_var();
	make_flt01_cnf(fvar(inode), f0_var, f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
  }

  SatVarId f0_var = ofvar(node, 0);
  SatVarId f1_var = ofvar(node, 1);
  SatVarId ovar = fvar(node);
  if ( f0_var == kSatVarIdIllegal ) {
    if ( f1_var == kSatVarIdIllegal ) {
      ;
    }
    else {
      ovar = solver().new_var();
      make_flt1_cnf(ovar, f1_var, fvar(node));
    }
  }
  else {
    if ( f1_var == kSatVarIdIllegal ) {
      ovar = solver().new_var();
      make_flt0_cnf(ovar, f0_var, fvar(node));
    }
    else {
      ovar = solver().new_var();
      make_flt01_cnf(ovar, f0_var, f1_var, fvar(node));
    }
  }

  if ( node->is_input() ) {
    SatLiteral glit(gvar(node), false);
    SatLiteral output(ovar, false);
    solver().add_clause( glit, ~output);
    solver().add_clause(~glit,  output);
  }
  else if ( node->is_output() ) {
    ASSERT_NOT_REACHED;
  }
  else {
    node->make_cnf(solver(), VectLitMap(ivars, ovar));
  }
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
MvalCnf::make_flt0_cnf(SatVarId ivar,
		       SatVarId fvar,
		       SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral flit(fvar, false);
  SatLiteral olit(ovar, false);

  solver().add_clause( ilit,        ~olit);
  solver().add_clause(       ~flit, ~olit);
  solver().add_clause(~ilit,  flit,  olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
MvalCnf::make_flt1_cnf(SatVarId ivar,
		       SatVarId fvar,
		       SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral flit(fvar, false);
  SatLiteral olit(ovar, false);

  solver().add_clause(~ilit,         olit);
  solver().add_clause(       ~flit,  olit);
  solver().add_clause( ilit,  flit, ~olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar0 故障変数
// @param[in] fvar1 故障変数
// @param[in] ovar 出力の変数
void
MvalCnf::make_flt01_cnf(SatVarId ivar,
			SatVarId fvar0,
			SatVarId fvar1,
			SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral f0lit(fvar0, false);
  SatLiteral f1lit(fvar1, false);
  SatLiteral olit(ovar, false);

  solver().add_clause(       ~f0lit,         ~olit);
  solver().add_clause(               ~f1lit,  olit);
  solver().add_clause( ilit,  f0lit,  f1lit, ~olit);
  solver().add_clause(~ilit,  f0lit,  f1lit,  olit);
}

END_NAMESPACE_YM_SATPG

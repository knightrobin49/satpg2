
/// @file SatEngine.cc
/// @brief SatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "MvalCnf.h"
#include "NodeValList.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "VidMap.h"
#include "VidLitMap.h"
#include "VectLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief バッファの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_buff_cnf(SatSolver& solver,
	      SatLiteral i,
	      SatLiteral o)
{
  solver.add_clause( i, ~o);
  solver.add_clause(~i,  o);
}

// @brief 2入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and2_cnf(SatSolver& solver,
	      SatLiteral i0,
	      SatLiteral i1,
	      SatLiteral o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause(~i0, ~i1, o);
}

// @brief 3入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and3_cnf(SatSolver& solver,
	      SatLiteral i0,
	      SatLiteral i1,
	      SatLiteral i2,
	      SatLiteral o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2, o);
}

// @brief 4入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2, i3 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and4_cnf(SatSolver& solver,
	      SatLiteral i0,
	      SatLiteral i1,
	      SatLiteral i2,
	      SatLiteral i3,
	      SatLiteral o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause( i3, ~o);
  solver.add_clause(~i0, ~i1, ~i2, ~i3, o);
}

// @brief 2入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or2_cnf(SatSolver& solver,
	     SatLiteral i0,
	     SatLiteral i1,
	     SatLiteral o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause( i0,  i1, ~o);
}

// @brief 3入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or3_cnf(SatSolver& solver,
	     SatLiteral i0,
	     SatLiteral i1,
	     SatLiteral i2,
	     SatLiteral o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause( i0,  i1, i2, ~o);
}

// @brief 4入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2, i3 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or4_cnf(SatSolver& solver,
	     SatLiteral i0,
	     SatLiteral i1,
	     SatLiteral i2,
	     SatLiteral i3,
	     SatLiteral o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause(~i3,  o);
  solver.add_clause( i0,  i1, i2, i3, ~o);
}

// @brief 2入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_xor2_cnf(SatSolver& solver,
	      SatLiteral i0,
	      SatLiteral i1,
	      SatLiteral o)
{
  solver.add_clause( i0, ~i1,  o);
  solver.add_clause(~i0,  i1,  o);
  solver.add_clause( i0,  i1, ~o);
  solver.add_clause(~i0, ~i1, ~o);
}

// @brief 3入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_xor3_cnf(SatSolver& solver,
	      SatLiteral i0,
	      SatLiteral i1,
	      SatLiteral i2,
	      SatLiteral o)
{
  solver.add_clause(~i0,  i1,  i2,  o);
  solver.add_clause( i0, ~i1,  i2,  o);
  solver.add_clause( i0,  i1, ~i2,  o);
  solver.add_clause( i0,  i1,  i2, ~o);
  solver.add_clause( i0, ~i1, ~i2, ~o);
  solver.add_clause(~i0,  i1, ~i2, ~o);
  solver.add_clause(~i0, ~i1,  i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2,  o);
}

END_NONAMESPACE


// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
SatEngine::SatEngine(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp) :
  mSolver(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
SatEngine::~SatEngine()
{
}

// @brief 複数故障検出回路のCNFを作る．
// @param[in] mval_cnf 故障回路用のデータ構造
// @param[in] fault_list 故障リスト
// @param[in] fnode_list 故障を持つノードのリスト
// @param[in] node_set 故障に関係するノード集合
void
SatEngine::make_mval_cnf(MvalCnf& mval_cnf,
			 const vector<const TpgFault*>& fault_list,
			 const vector<const TpgNode*>& fnode_list,
			 const NodeSet& node_set)
{
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId gvar = new_var();
    mval_cnf.set_gvar(node, gvar);
    mval_cnf.set_fvar(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    SatVarId fvar = new_var();
    SatVarId dvar = new_var();
    mval_cnf.set_fvar(node, fvar);
    mval_cnf.set_dvar(node, dvar);
  }

  // 故障を活性化するとき true にする変数．
  ymuint nf = fault_list.size();
  mval_cnf.set_fault_num(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SatVarId fdvar = new_var();
    mval_cnf.set_fault_var(i, fdvar);
    const TpgFault* f = fault_list[i];
    int fval = f->val();
    const TpgNode* node = f->tpg_node();

    if ( f->is_output_fault() ) {
      mval_cnf.set_ofvar(node, fval, fdvar);
    }
    else {
      ymuint pos = f->tpg_pos();
      mval_cnf.set_ifvar(node, pos, fval, fdvar);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    make_node_cnf(node, mval_cnf.gvar_map());
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( mval_cnf.has_faults(node) ) {
      make_fnode_cnf(node, mval_cnf);
    }
    else {
      make_node_cnf(node, mval_cnf.fvar_map());
    }

    make_dchain_cnf(node, node_set.dom_node(), mval_cnf.gvar_map(), mval_cnf.fvar_map(), mval_cnf.dvar_map());
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    SatLiteral dlit(mval_cnf.dvar(node));
    tmp_lits_add(dlit);
  }
  tmp_lits_end();
}

// @brief ノードの入出力の関係を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] vid_map 変数番号のマップ
void
SatEngine::make_node_cnf(const TpgNode* node,
			 const VidMap& vid_map)
{
  node->make_cnf(mSolver, VidLitMap(node, vid_map));
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
SatEngine::make_fnode_cnf(const TpgNode* node,
			  const MvalCnf& mval_cnf)
{
  const VidMap& gvar_map = mval_cnf.gvar_map();
  const VidMap& fvar_map = mval_cnf.fvar_map();

  ymuint ni = node->fanin_num();
  vector<SatVarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    SatVarId f0_var = mval_cnf.ifvar(node, i, 0);
    SatVarId f1_var = mval_cnf.ifvar(node, i, 1);
    if ( f0_var == kSatVarIdIllegal ) {
      if ( f1_var == kSatVarIdIllegal ) {
	ivars[i] = fvar_map(inode);
      }
      else {
	SatVarId tmp_var = new_var();
	make_flt1_cnf(fvar_map(inode), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kSatVarIdIllegal ) {
	SatVarId tmp_var = new_var();
	make_flt0_cnf(fvar_map(inode), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	SatVarId tmp_var = new_var();
	make_flt01_cnf(fvar_map(inode), f0_var, f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
  }

  SatVarId f0_var = mval_cnf.ofvar(node, 0);
  SatVarId f1_var = mval_cnf.ofvar(node, 1);
  SatVarId ovar = fvar_map(node);
  if ( f0_var == kSatVarIdIllegal ) {
    if ( f1_var == kSatVarIdIllegal ) {
      ;
    }
    else {
      ovar = new_var();
      make_flt1_cnf(ovar, f1_var, fvar_map(node));
    }
  }
  else {
    if ( f1_var == kSatVarIdIllegal ) {
      ovar = new_var();
      make_flt0_cnf(ovar, f0_var, fvar_map(node));
    }
    else {
      ovar = new_var();
      make_flt01_cnf(ovar, f0_var, f1_var, fvar_map(node));
    }
  }

  if ( node->is_input() ) {
    SatLiteral glit(gvar_map(node), false);
    SatLiteral output(ovar, false);
    add_clause( glit, ~output);
    add_clause(~glit,  output);
  }
  else if ( node->is_output() ) {
    ASSERT_NOT_REACHED;
  }
  else {
    node->make_cnf(mSolver, VectLitMap(ivars, ovar));
  }
}

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] dst_node 伝搬条件の終点のノード
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
// @param[in] dvar_map 故障伝搬条件の変数マップ
void
SatEngine::make_dchain_cnf(const TpgNode* node,
			   const TpgNode* dst_node,
			   const VidMap& gvar_map,
			   const VidMap& fvar_map,
			   const VidMap& dvar_map)
{
  SatLiteral glit(gvar_map(node), false);
  SatLiteral flit(fvar_map(node), false);
  SatLiteral dlit(dvar_map(node), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  add_clause(~glit, ~flit, ~dlit);
  add_clause( glit,  flit, ~dlit);

  if ( node->is_output() || node == dst_node ) {
    // 出力ノードの場合，XOR(glit, flit) -> dlit となる．
    add_clause(~glit,  flit, dlit);
    add_clause( glit, ~flit, dlit);
  }
  else {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    tmp_lits_begin(nfo + 1);
    tmp_lits_add(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      const TpgNode* onode = node->active_fanout(j);
      SatLiteral odlit(dvar_map(onode), false);
      tmp_lits_add(odlit);
    }
    tmp_lits_end();
  }
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt0_cnf(SatVarId ivar,
			 SatVarId fvar,
			 SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral flit(fvar, false);
  SatLiteral olit(ovar, false);

  add_clause( ilit,        ~olit);
  add_clause(       ~flit, ~olit);
  add_clause(~ilit,  flit,  olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt1_cnf(SatVarId ivar,
			 SatVarId fvar,
			 SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral flit(fvar, false);
  SatLiteral olit(ovar, false);

  add_clause(~ilit,         olit);
  add_clause(       ~flit,  olit);
  add_clause( ilit,  flit, ~olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar0 故障変数
// @param[in] fvar1 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt01_cnf(SatVarId ivar,
			  SatVarId fvar0,
			  SatVarId fvar1,
			  SatVarId ovar)
{
  SatLiteral ilit(ivar, false);
  SatLiteral f0lit(fvar0, false);
  SatLiteral f1lit(fvar1, false);
  SatLiteral olit(ovar, false);

  add_clause(       ~f0lit,         ~olit);
  add_clause(               ~f1lit,  olit);
  add_clause( ilit,  f0lit,  f1lit, ~olit);
  add_clause(~ilit,  f0lit,  f1lit,  olit);
}

// @brief 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_and_cnf(const LitMap& litmap,
			bool inv)
{
  ymuint n = litmap.input_size();
  SatLiteral output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_and2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_and3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_and4_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<SatLiteral> tmp(n + 1);
  tmp_lits_begin(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    SatLiteral input = litmap.input(i);
    add_clause(input, ~output);
    tmp_lits_add(~input);
  }
  tmp_lits_add(output);
  tmp_lits_end();
}

// @brief 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_or_cnf(const LitMap& litmap,
		       bool inv)
{
  ymuint n = litmap.input_size();
  SatLiteral output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_or2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_or3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_or4_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  tmp_lits_begin(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    SatLiteral input = litmap.input(i);
    add_clause(~input, output);
    tmp_lits_add(input);
  }
  tmp_lits_add(~output);
  tmp_lits_end();
}

// @brief 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_xor_cnf(const LitMap& litmap,
			bool inv)
{
  ymuint n = litmap.input_size();
  SatLiteral output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_xor2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_xor3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  default: break;
  }

  SatVarId tmp_var = new_var();
  SatLiteral tmp_lit(tmp_var, false);
  make_xor2_cnf(mSolver, litmap.input(0), litmap.input(1), tmp_lit);

  for (ymuint i = 2; i < n; ++ i) {
    SatLiteral tmp_out;
    if ( i == n - 1 ) {
      tmp_out = output;
    }
    else {
      SatVarId tmp_var = new_var();
      tmp_out = SatLiteral(tmp_var, false);
    }
    make_xor2_cnf(mSolver, litmap.input(i), tmp_lit, tmp_out);
    tmp_lit = tmp_out;
  }
}

END_NAMESPACE_YM_SATPG

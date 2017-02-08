
/// @file DtpgS.cc
/// @brief DtpgS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DtpgS.h"
#include "sa/DtpgStats.h"
#include "sa/StructSat.h"
#include "sa/FoCone.h"
#include "TpgFault.h"
#include "TpgNetwork.h"

#include "LitMap.h"
#include "VidLitMap.h"
#include "VectLitMap.h"

#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG_SA

BEGIN_NONAMESPACE

void
make_dchain_cnf(SatSolver& solver,
		const TpgNode* node,
		const VidMap& gvar_map,
		const VidMap& fvar_map,
		const VidMap& dvar_map)
{
  SatLiteral glit(gvar_map(node));
  SatLiteral flit(fvar_map(node));
  SatLiteral dlit(dvar_map(node));

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  if ( node->is_ppo() ) {
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
  }
  else {
    // dlit -> ファンアウト先のノードの dlit の一つが 1
    ymuint nfo = node->fanout_num();
    if ( nfo == 1 ) {
      SatLiteral odlit(dvar_map(node->fanout(0)));
      solver.add_clause(~dlit, odlit);
    }
    else {
      vector<SatLiteral> tmp_lits(nfo + 1);
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* onode = node->fanout(i);
	tmp_lits[i] = SatLiteral(dvar_map(onode));
      }
      tmp_lits[nfo] = ~dlit;
      solver.add_clause(tmp_lits);
    }
    const TpgNode* imm_dom = node->imm_dom();
    if ( imm_dom != nullptr ) {
      SatLiteral odlit(dvar_map(imm_dom));
      solver.add_clause(~dlit, odlit);
    }
  }
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] network 対象のネットワーク
DtpgS::DtpgS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     const TpgNetwork& network) :
  DtpgBase(bt),
  mSatType(sat_type),
  mSatOption(sat_option),
  mSatOutP(sat_outp),
  mMaxNodeId(network.node_num())
{
}

// @brief デストラクタ
DtpgS::~DtpgS()
{
}

// @brief テスト生成を行なう．
// @param[in] fault 対象の故障
// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
// @param[inout] stats DTPGの統計情報
// @return 結果を返す．
SatBool3
DtpgS::dtpg(const TpgFault* fault,
	    NodeValList& nodeval_list,
	    DtpgStats& stats)
{
  cnf_begin();

  mNodeList.clear();
  mMarkArray.clear();
  mMarkArray.resize(mMaxNodeId, 0);

  const TpgNode* fnode = fault->tpg_onode();

  vector<const TpgNode*> output_list;

  // fnode の TFO を mNodeList に入れる．
  set_tfo_mark(fnode);
  if ( fnode->is_ppo() ) {
    output_list.push_back(fnode);
  }
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint nfo = node->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* onode = node->fanout(i);
      set_tfo_mark(onode);
      if ( onode->is_ppo() ) {
	output_list.push_back(onode);
      }
    }
  }
  ymuint tfo_num = mNodeList.size();

  // TFO の TFI を mNodeList に入れる．
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      set_tfi_mark(inode);
    }
  }
  ymuint tfi_num = mNodeList.size();

  SatSolver solver(mSatType, mSatOption, mSatOutP);

  // TFO の部分に変数を割り当てる．
  GenVidMap gvar_map(mMaxNodeId);
  GenVidMap fvar_map(mMaxNodeId);
  GenVidMap dvar_map(mMaxNodeId);
  for (ymuint rpos = 0; rpos < tfo_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    SatVarId gvar = solver.new_var();
    SatVarId fvar = solver.new_var();
    SatVarId dvar = solver.new_var();

    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  // TFI の部分に変数を割り当てる．
  for (ymuint rpos = tfo_num; rpos < tfi_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    SatVarId gvar = solver.new_var();

    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  for (ymuint i = 0; i < tfi_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    node->make_cnf(solver, VidLitMap(node, gvar_map));
  }


  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  if ( fault->is_stem_fault() ) {
    SatLiteral flit(fvar_map(fnode));
    if ( fault->val() == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
    }
  }
  else {
    ymuint ni = fnode->fanin_num();
    vector<SatVarId> ivars(ni);
    SatVarId fvar = solver.new_var();
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fault->tpg_pos() ) {
	ivars[i] = fvar;
      }
      else {
	const TpgNode* inode = fnode->fanin(i);
	ivars[i] = fvar_map(inode);
      }
    }
    SatLiteral flit(fvar);
    if ( fault->val() == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
    }
    fnode->make_cnf(solver, VectLitMap(ivars, fvar_map(fnode)));
  }
  make_dchain_cnf(solver, fnode, gvar_map, fvar_map, dvar_map);

  for (ymuint i = 1; i < tfo_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    node->make_cnf(solver, VidLitMap(node, fvar_map));

    make_dchain_cnf(solver, node, gvar_map, fvar_map, dvar_map);

    ymuint ni = node->fanin_num();
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(ni + 1);
    SatLiteral dlit(dvar_map(node));
    tmp_lits.push_back(~dlit);
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      if ( gvar_map(inode) != fvar_map(inode) ) {
	SatLiteral dlit(dvar_map(inode));
	tmp_lits.push_back(dlit);
      }
    }
    solver.add_clause(tmp_lits);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint no = output_list.size();
  vector<SatLiteral> odiff(no);
  for (ymuint i = 0; i < no; ++ i) {
    const TpgNode* node = output_list[i];
    SatLiteral dlit(dvar_map(node));
    odiff[i] = dlit;
  }
  solver.add_clause(odiff);

  cnf_end(stats);

  vector<SatLiteral> assumptions;
  for (const TpgNode* node = fnode; node != nullptr; node = node->imm_dom()) {
    SatLiteral dlit(dvar_map(node));
    assumptions.push_back(dlit);
  }

  SatBool3 ans = solve(solver, assumptions, fault, fnode, output_list,
		       gvar_map, fvar_map, nodeval_list, stats);

  return ans;
}

END_NAMESPACE_YM_SATPG_SA

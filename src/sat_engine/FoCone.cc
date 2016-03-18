
/// @file FoCone.cc
/// @brief FoCone の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FoCone.h"
#include "StructSat.h"
#include "TpgNode.h"
#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

bool debug = false;

struct Lt
{
  bool
  operator()(const TpgNode* left,
	     const TpgNode* right)
  {
    return left->output_id2() < right->output_id2();
  }
};

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] struct_sat StructSat ソルバ
// @param[in] fnode 故障位置のノード
// @param[in] detect 検出条件
FoCone::FoCone(StructSat& struct_sat,
	       const TpgNode* fnode,
	       Val3 detect) :
  mStructSat(struct_sat),
  mMaxNodeId(struct_sat.max_node_id()),
  mMarkArray(max_id()),
  mFvarMap(max_id()),
  mDvarMap(max_id())
{
  mNodeList.reserve(max_id());

  // 故障のあるノードの TFO を mNodeList に入れる．
  set_mark(fnode);
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* fonode = node->active_fanout(i);
      if ( !mark(fonode) ) {
	set_mark(fonode);
      }
    }
  }
  ymuint tfo_num = mNodeList.size();

  // mNodeList に含まれるノードの TFI を mNodeList に追加する．
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      if ( !mark(inode) ) {
	set_mark(inode);
      }
    }
  }

  // 出力のリストを整列しておく．
  sort(mOutputList.begin(), mOutputList.end(), Lt());

  // focone に含まれるノードに変数を割り当てる．
  for (ymuint i = 0; i < tfo_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    mStructSat.make_cnf(node);
    SatVarId fvar = solver().new_var();
    SatVarId dvar = solver().new_var();
    set_fvar(node, fvar);
    set_dvar(node, dvar);
    if ( debug ) {
      cout << "fvar(" << node->name() << ") = " << fvar << endl;
    }
  }

  // focone に含まれないノードの fvar を gvar にする．
  for (ymuint i = tfo_num; i < mNodeList.size(); ++ i) {
    const TpgNode* node = mNodeList[i];
    set_fvar(node, gvar(node));
  }

  for (ymuint i = 0; i < tfo_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    if ( node != fnode ) {
      // 故障回路のゲートの入出力関係を表すCNFを作る．
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(node, nullptr);
  }

  ymuint npo = mOutputList.size();

  if ( detect == kVal0 ) {
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = mOutputList[i];
      SatLiteral dlit(dvar(node));
      solver().add_clause(~dlit);
    }
  }
  else if ( detect == kVal1 ) {
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(npo + 1);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = mOutputList[i];
      SatLiteral dlit(dvar(node));
      tmp_lits.push_back(dlit);
    }
    solver().add_clause(tmp_lits);

    for (const TpgNode* node = fnode; node != nullptr && node != nullptr; node = node->imm_dom()) {
      SatLiteral dlit(dvar(node));
      solver().add_clause(dlit);
    }
  }
}

// @brief デストラクタ
FoCone::~FoCone()
{
}

// @brief 正常値と故障値が異なるという制約を追加する．
// @param[in] node 対象のノード
void
FoCone::add_diff_clause(const TpgNode* node)
{
  SatLiteral lit1(gvar(node));
  SatLiteral lit2(fvar(node));
  solver().add_clause( lit1,  lit2);
  solver().add_clause(~lit1, ~lit2);
}

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] dst_node 伝搬条件の終点のノード
void
FoCone::make_dchain_cnf(const TpgNode* node,
			const TpgNode* dst_node)
{
  SatLiteral glit(gvar(node), false);
  SatLiteral flit(fvar(node), false);
  SatLiteral dlit(dvar(node), false);

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
      SatLiteral odlit(dvar(onode), false);
      tmp_lits.push_back(odlit);
    }
    solver().add_clause(tmp_lits);
  }
}

END_NAMESPACE_YM_SATPG

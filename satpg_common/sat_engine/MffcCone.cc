
/// @file MffcCone.cc
/// @brief MffcCone の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MffcCone.h"
#include "StructSat.h"
#include "TpgNode.h"
#include "VectLitMap.h"
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
MffcCone::MffcCone(StructSat& struct_sat,
		   const TpgNode* fnode) :
  mStructSat(struct_sat),
  mMaxNodeId(struct_sat.max_node_id()),
  mMarkArray(max_id()),
  mElemList(fnode->mffc_elem_num()),
  mFvarMap(max_id()),
  mDvarMap(max_id()),
  mElemVarList(fnode->mffc_elem_num())
{
  mNodeList.reserve(max_id());

  for (ymuint i = 0; i < fnode->mffc_elem_num(); ++ i) {
    mElemList[i] = fnode->mffc_elem(i);
    mElemVarList[i] = solver().new_var();
  }

  // mElemList に含まれるノードの TFO を mNodeList に加える．
  vector<int> elem_map(max_id(), -1);
  for (ymuint i = 0; i < mffc_elem_num(); ++ i) {
    const TpgNode* node = mffc_elem(i);
    set_mark(node);
    elem_map[node->id()] = i;
  }
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
  mTfoNum = mNodeList.size();

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
  for (ymuint i = 0; i < mTfoNum; ++ i) {
    const TpgNode* node = mNodeList[i];
    mStructSat.make_tfi_cnf(node);
    SatVarId fvar = solver().new_var();
    SatVarId dvar = solver().new_var();
    set_fvar(node, fvar);
    set_dvar(node, dvar);
    if ( debug ) {
      cout << "fvar(" << node->name() << ") = " << fvar << endl;
    }
  }

  // focone に含まれないノードの fvar を gvar にする．
  for (ymuint i = mTfoNum; i < mNodeList.size(); ++ i) {
    const TpgNode* node = mNodeList[i];
    set_fvar(node, gvar(node));
  }

  for (ymuint i = 0; i < mTfoNum; ++ i) {
    const TpgNode* node = mNodeList[i];
    int fpos = elem_map[node->id()];
    if ( fpos >= 0 ) {
      // 出力に故障挿入変数との XOR ゲートを挿入する．
      SatVarId tmp_var = solver().new_var();
      ymuint ni = node->fanin_num();
      vector<SatVarId> tmp_ivars(ni);
      for (ymuint j = 0; j < ni; ++ j) {
	tmp_ivars[j] = fvar(node->fanin(j));
      }
      VectLitMap lit_map(tmp_ivars, tmp_var);
      node->make_cnf(solver(), lit_map);

      SatLiteral ilit(tmp_var);
      SatLiteral olit(fvar(node));
      SatLiteral dlit(mElemVarList[fpos]);
      solver().add_clause( ilit,  dlit, ~olit);
      solver().add_clause( ilit, ~dlit,  olit);
      solver().add_clause(~ilit,  dlit,  olit);
      solver().add_clause(~ilit, ~dlit, ~olit);
    }
    else {
      // 故障回路のゲートの入出力関係を表すCNFを作る．
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }

    // D-Chain 制約を作る．
    mStructSat.make_dchain_cnf(node, nullptr, fvar_map(), dvar_map());
  }

  ymuint npo = mOutputList.size();
  vector<SatLiteral> tmp_lits;
  tmp_lits.reserve(npo);
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

// @brief デストラクタ
MffcCone::~MffcCone()
{
}

// @brief 故障挿入位置を選ぶ．
// @param[in] pos 位置番号 ( 0 <= pos < mffc_elem_num() )
// @param[out] assumptions 結果の割り当てを追加するベクタ
void
MffcCone::select_fault_node(ymuint pos,
			    vector<SatLiteral>& assumptions)
{
  ASSERT_COND( pos < mElemList.size() );

  // mElemVar の設定を行う．
  for (ymuint i = 0; i < mElemList.size(); ++ i) {
    SatVarId fdvar = mElemVarList[i];
    if ( i == pos ) {
      assumptions.push_back(SatLiteral(fdvar, false));
    }
    else {
      assumptions.push_back(SatLiteral(fdvar, true));
    }
  }

  // node1 の TFO でない部分の dvar を false にする．
  const TpgNode* node0 = mElemList[0];
  const TpgNode* node1 = mElemList[pos];
  vector<const TpgNode*> node_list;
  vector<bool> mark(max_id(), false);
  node_list.push_back(node1);
  mark[node1->id()] = true;
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    const TpgNode* node = node_list[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* onode = node->active_fanout(i);
      if ( mark[onode->id()] ) {
	continue;
      }
      mark[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
  for (ymuint i = 0; i < mTfoNum; ++ i) {
    const TpgNode* node = mNodeList[i];
    if ( !mark[node->id()] ) {
      SatLiteral dlit(dvar(node), true);
      assumptions.push_back(dlit);
    }
  }
}

END_NAMESPACE_YM_SATPG

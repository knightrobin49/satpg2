
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

#include "NodeValList.h"

BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
// @param[in] struct_sat StructSat ソルバ
// @param[in] fnode 故障位置のノード
// @param[in] bnode ブロックノード
// @param[in] detect 検出条件
//
// ブロックノードより先のノードは含めない．
// 通常 bnode は fnode の dominator
// となっているはず．
FoCone::FoCone(StructSat& struct_sat,
	       const TpgNode* fnode,
	       const TpgNode* bnode,
	       Val3 detect) :
  ConeBase(struct_sat)
{
  if ( bnode != nullptr ) {
    set_end_mark(bnode);
  }

  mark_tfo_tfi(fnode);

  for (ymuint i = 0; i < tfo_num(); ++ i) {
    const TpgNode* node = tfo_node(i);
    if ( node != fnode ) {
      // 故障回路のゲートの入出力関係を表すCNFを作る．
      node->make_cnf(solver(), VidLitMap(node, fvar_map()));
    }

    if ( detect == kVal1 ) {
      // D-Chain 制約を作る．
      make_dchain_cnf(node);
    }
  }

  ymuint npo = output_num();

  if ( detect == kVal0 ) {
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_node(i);
      SatLiteral glit(gvar(node), false);
      SatLiteral flit(fvar(node), false);
      solver().add_clause(~glit,  flit);
      solver().add_clause( glit, ~flit);
    }
  }
  else if ( detect == kVal1 ) {
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(npo);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_node(i);
      SatLiteral dlit(dvar(node));
      tmp_lits.push_back(dlit);
    }
    solver().add_clause(tmp_lits);

    SatLiteral dlit(dvar(fnode));
    solver().add_clause(dlit);
  }
}

// @brief デストラクタ
FoCone::~FoCone()
{
}

END_NAMESPACE_YM_SATPG

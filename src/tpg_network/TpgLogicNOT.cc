
/// @file TpgLogicNOT.cc
/// @brief TpgLogicNOT の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicNOT.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNOT
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] inode ファンインのノード
TpgLogicNOT::TpgLogicNOT(ymuint id,
			 TpgNode* inode) :
  TpgLogic1(id, inode)
{
}

// @brief デストラクタ
TpgLogicNOT::~TpgLogicNOT()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicNOT::gate_type() const
{
  return kGateNOT;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLodicNOT::make_cnf(SatSolver& solver,
		      const LitMap& lit_map)
{
  SatLiteral ilit = lit_map.input(0);
  SatLiteral olit = lit_map.output();
  solver.add_clause(~ilit, ~olit);
  solver.add_clause( ilit,  olit);
}

END_NAMESPACE_YM_SATPG

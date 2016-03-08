
/// @file TpgLogicC0.cc
/// @brief TpgLogicC0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicC0.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogcC0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
TpgLogicC0::TpgLogicC0(ymuint id,
		       const char* name) :
  TpgLogic0(id, name)
{
}

// @brief デストラクタ
TpgLogicC0::~TpgLogicC0()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicC0::gate_type() const
{
  return kGateCONST0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicC0::make_cnf(SatSolver& solver,
		     const LitMap& lit_map)
{
  solver.add_clause(~lit_map.output());
}

END_NAMESPACE_YM_SATPG

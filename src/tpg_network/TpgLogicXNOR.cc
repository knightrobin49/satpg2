
/// @file TpgLogicXNOR.cc
/// @brief TpgLogicXNOR[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicXNOR.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicXNOR2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] inode0, inode1 ファンインのノード
TpgLogicXNOR2::TpgLogicXNOR2(ymuint id,
			     TpgNode* inode0,
			     TpgNode* inode1) :
  TpgLogic2(id, inode0, inode1)
{
}

// @brief デストラクタ
TpgLogicXNOR2::~TpgLogicXNOR2()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicXNOR2::gate_type() const
{
  return kGateXNOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXNOR2::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXNOR2::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXNOR2::coval() const
{
  return kValX;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXNOR2::noval() const
{
  return kValX;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLodicXNOR2::make_cnf(SatSolver& solver,
			const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0, ~ilit1,  olit);
  solver.add_clause(~ilit1,  ilit1, ~olit);
  solver.add_clause( ilit1, ~ilit1, ~olit);
  solver.add_clause( ilit0,  ilit1,  olit);
}

END_NAMESPACE_YM_SATPG

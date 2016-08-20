
/// @file TpgLogicXOR.cc
/// @brief TpgLogicXOR[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicXOR.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicXOR2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgLogicXOR2::TpgLogicXOR2(ymuint id,
			   const vector<TpgNode*>& fanin_list,
			   ymuint fanout_num) :
  TpgNode(id, fanin_list, fanout_num)
{
}

// @brief デストラクタ
TpgLogicXOR2::~TpgLogicXOR2()
{
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXOR2::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXOR2::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXOR2::coval() const
{
  return kValX;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicXOR2::noval() const
{
  return kValX;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicXOR2::make_cnf(SatSolver& solver,
		      const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0, ~ilit1, ~olit);
  solver.add_clause(~ilit0,  ilit1,  olit);
  solver.add_clause( ilit0, ~ilit1,  olit);
  solver.add_clause( ilit0,  ilit1, ~olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicXOR2::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ymuint pos = (fpos == 0) ? 1 : 0;
  SatLiteral ilit0 = lit_map.input(pos);
  SatLiteral olit  = lit_map.output();
  if ( fval == 0 ) {
    solver.add_clause(~ilit0,  olit);
    solver.add_clause( ilit0, ~olit);
  }
  else {
    solver.add_clause(~ilit0, ~olit);
    solver.add_clause( ilit0,  olit);
  }
}

END_NAMESPACE_YM_SATPG

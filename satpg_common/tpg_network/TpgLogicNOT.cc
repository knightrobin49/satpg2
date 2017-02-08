
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
// @param[in] fanin ファンイン
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
TpgLogicNOT::TpgLogicNOT(ymuint id,
			 TpgNode* fanin,
			 ymuint fanout_num,
			 TpgNode** fanout_list) :
  TpgLogic1(id, fanin, fanout_num, fanout_list)
{
}

// @brief デストラクタ
TpgLogicNOT::~TpgLogicNOT()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNOT::gate_type() const
{
  return kGateNOT;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOT::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOT::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOT::coval() const
{
  return kValX;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOT::noval() const
{
  return kValX;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNOT::make_cnf(SatSolver& solver,
		      const LitMap& lit_map) const
{
  SatLiteral ilit = lit_map.input(0);
  SatLiteral olit = lit_map.output();
  solver.add_clause(~ilit, ~olit);
  solver.add_clause( ilit,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNOT::make_faulty_cnf(SatSolver& solver,
			     ymuint fpos,
			     int fval,
			     const LitMap& lit_map) const
{
  SatLiteral olit = lit_map.output();
  if ( fval == 0 ) {
    solver.add_clause( olit);
  }
  else {
    solver.add_clause(~olit);
  }
}

END_NAMESPACE_YM_SATPG

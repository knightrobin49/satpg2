
/// @file TpgLogicAND.cc
/// @brief TpgLogicAND[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicAND.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicAND
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgLogicAND::TpgLogicAND(ymuint id,
			 const vector<TpgNode*>& fanin_list,
			 ymuint fanout_num) :
  TpgNode(id, fanin_list, fanout_num)
{
}

// @brief デストラクタ
TpgLogicAND::~TpgLogicAND()
{
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicAND::cval() const
{
  return kVal0;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicAND::nval() const
{
  return kVal1;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicAND::coval() const
{
  return kVal0;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicAND::noval() const
{
  return kVal1;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicAND::make_cnf(SatSolver& solver,
		      const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  ymuint ni = fanin_num();
  vector<SatLiteral> tmp_lits(ni + 1);
  for (ymuint i = 0; i < ni; ++ i) {
    SatLiteral ilit = lit_map.input(i);
    solver.add_clause(ilit, ~olit);
    tmp_lits[i] = ~ilit;
  }
  tmp_lits[ni] = olit;
  solver.add_clause(tmp_lits);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicAND::make_faulty_cnf(SatSolver& solver,
			     ymuint fpos,
			     int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );
  SatLiteral olit  = lit_map.output();
  ymuint ni = fanin_num();
  vector<SatLiteral> tmp_lits;
  tmp_lits.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    if ( i == fpos ) {
      continue;
    }
    SatLiteral ilit = lit_map.input(i);
    solver.add_clause(ilit, ~olit);
    tmp_lits.push_back(~ilit);
  }
  tmp_lits.push_back(olit);
  solver.add_clause(tmp_lits);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicAND2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgLogicAND2::TpgLogicAND2(ymuint id,
			   const vector<TpgNode*>& fanin_list,
			   ymuint fanout_num) :
  TpgLogicAND(id, fanin_list, fanout_num)
{
}

// @brief デストラクタ
TpgLogicAND2::~TpgLogicAND2()
{
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicAND2::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);

  solver.add_clause( ilit0,         ~olit);
  solver.add_clause(         ilit1, ~olit);

  solver.add_clause(~ilit0, ~ilit1,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicAND2::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );

  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0;

  switch ( fpos ) {
  case 0: ilit0 = lit_map.input(1); break;
  case 1: ilit0 = lit_map.input(0); break;
  }

  solver.add_clause( ilit0, ~olit);

  solver.add_clause(~ilit0,  olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicAND3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgLogicAND3::TpgLogicAND3(ymuint id,
			   const vector<TpgNode*>& fanin_list,
			   ymuint fanout_num) :
  TpgLogicAND(id, fanin_list, fanout_num)
{
}

// @brief デストラクタ
TpgLogicAND3::~TpgLogicAND3()
{
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicAND3::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);

  solver.add_clause( ilit0,                 ~olit);
  solver.add_clause(         ilit1,         ~olit);
  solver.add_clause(                 ilit2, ~olit);

  solver.add_clause(~ilit0, ~ilit1, ~ilit2,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicAND3::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );

  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0;
  SatLiteral ilit1;

  switch ( fpos ) {
  case 0:
    ilit0 = lit_map.input(1);
    ilit1 = lit_map.input(2);
    break;

  case 1:
    ilit0 = lit_map.input(0);
    ilit1 = lit_map.input(2);
    break;

  case 2:
    ilit0 = lit_map.input(0);
    ilit1 = lit_map.input(1);
    break;
  }

  solver.add_clause( ilit0,         ~olit);
  solver.add_clause(         ilit1, ~olit);

  solver.add_clause(~ilit0, ~ilit1,  olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicAND4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgLogicAND4::TpgLogicAND4(ymuint id,
			   const vector<TpgNode*>& fanin_list,
			   ymuint fanout_num) :
  TpgLogicAND(id, fanin_list, fanout_num)
{
}

// @brief デストラクタ
TpgLogicAND4::~TpgLogicAND4()
{
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicAND4::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(3);

  solver.add_clause( ilit0,                         ~olit);
  solver.add_clause(         ilit1,                 ~olit);
  solver.add_clause(                 ilit2,         ~olit);
  solver.add_clause(                         ilit3, ~olit);

  solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~ilit3,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicAND4::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );

  SatLiteral olit  = lit_map.output();
  SatLiteral ilit0;
  SatLiteral ilit1;
  SatLiteral ilit2;

  switch ( fpos ) {
  case 0:
    ilit0 = lit_map.input(1);
    ilit1 = lit_map.input(2);
    ilit2 = lit_map.input(3);
    break;

  case 1:
    ilit0 = lit_map.input(0);
    ilit1 = lit_map.input(2);
    ilit2 = lit_map.input(3);
    break;

  case 2:
    ilit0 = lit_map.input(0);
    ilit1 = lit_map.input(1);
    ilit2 = lit_map.input(3);
    break;

  case 3:
    ilit0 = lit_map.input(0);
    ilit1 = lit_map.input(1);
    ilit2 = lit_map.input(2);
    break;
  }

  solver.add_clause( ilit0,                 ~olit);
  solver.add_clause(         ilit1,         ~olit);
  solver.add_clause(                 ilit2, ~olit);

  solver.add_clause(~ilit0, ~ilit1, ~ilit2,  olit);
}

END_NAMESPACE_YM_SATPG

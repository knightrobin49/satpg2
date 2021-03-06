
/// @file TpgLogicNAND.cc
/// @brief TpgLogicNAND[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicNAND.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNAND2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1 ファンインのノード
TpgLogicNAND2::TpgLogicNAND2(ymuint id,
			     const char* name,
			     TpgNode* inode0,
			     TpgNode* inode1) :
  TpgLogic2(id, name, inode0, inode1)
{
}

// @brief デストラクタ
TpgLogicNAND2::~TpgLogicNAND2()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNAND2::gate_type() const
{
  return kGateNAND;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND2::cval() const
{
  return kVal0;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND2::nval() const
{
  return kVal1;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND2::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND2::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNAND2::make_cnf(SatSolver& solver,
			const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,          olit);
  solver.add_clause( ilit1,          olit);
  solver.add_clause(~ilit0, ~ilit1, ~olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNAND2::make_faulty_cnf(SatSolver& solver,
			       ymuint fpos,
			       int fval,
			       const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );
  ymuint pos = (fpos == 0) ? 1 : 0;
  SatLiteral ilit0 = lit_map.input(pos);
  SatLiteral olit = lit_map.output();
  solver.add_clause( ilit0,  olit);
  solver.add_clause(~ilit0, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNAND3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2 ファンインのノード
TpgLogicNAND3::TpgLogicNAND3(ymuint id,
			     const char* name,
			     TpgNode* inode0,
			     TpgNode* inode1,
			     TpgNode* inode2) :
  TpgLogic3(id, name, inode0, inode1, inode2)
{
}

// @brief デストラクタ
TpgLogicNAND3::~TpgLogicNAND3()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNAND3::gate_type() const
{
  return kGateNAND;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND3::cval() const
{
  return kVal0;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND3::nval() const
{
  return kVal1;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND3::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND3::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNAND3::make_cnf(SatSolver& solver,
			const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,                  olit);
  solver.add_clause( ilit1,                  olit);
  solver.add_clause( ilit2,                  olit);
  solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNAND3::make_faulty_cnf(SatSolver& solver,
			       ymuint fpos,
			       int fval,
			       const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );
  ymuint pos0;
  ymuint pos1;
  switch ( fpos ) {
  case 0:
    pos0 = 1;
    pos1 = 2;
    break;
  case 1:
    pos0 = 0;
    pos1 = 2;
    break;
  case 2:
    pos0 = 0;
    pos1 = 1;
    break;
  }
  SatLiteral ilit0 = lit_map.input(pos0);
  SatLiteral ilit1 = lit_map.input(pos1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,          olit);
  solver.add_clause( ilit1,          olit);
  solver.add_clause(~ilit0, ~ilit1, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNAND4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
TpgLogicNAND4::TpgLogicNAND4(ymuint id,
			     const char* name,
			     TpgNode* inode0,
			     TpgNode* inode1,
			     TpgNode* inode2,
			     TpgNode* inode3) :
  TpgLogic4(id, name, inode0, inode1, inode2, inode3)
{
}

// @brief デストラクタ
TpgLogicNAND4::~TpgLogicNAND4()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNAND4::gate_type() const
{
  return kGateNAND;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND4::cval() const
{
  return kVal0;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND4::nval() const
{
  return kVal1;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND4::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNAND4::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNAND4::make_cnf(SatSolver& solver,
			const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(3);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,                          olit);
  solver.add_clause( ilit1,                          olit);
  solver.add_clause( ilit2,                          olit);
  solver.add_clause( ilit3,                          olit);
  solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~ilit3, ~olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNAND4::make_faulty_cnf(SatSolver& solver,
			       ymuint fpos,
			       int fval,
			       const LitMap& lit_map) const
{
  ASSERT_COND( fval == 1 );
  ymuint pos0;
  ymuint pos1;
  ymuint pos2;
  switch ( fpos ) {
  case 0:
    pos0 = 1;
    pos1 = 2;
    pos2 = 3;
    break;
  case 1:
    pos0 = 0;
    pos1 = 2;
    pos2 = 3;
    break;
  case 2:
    pos0 = 0;
    pos1 = 1;
    pos2 = 3;
    break;
  case 3:
    pos0 = 0;
    pos1 = 1;
    pos2 = 2;
    break;
  }
  SatLiteral ilit0 = lit_map.input(pos0);
  SatLiteral ilit1 = lit_map.input(pos1);
  SatLiteral ilit2 = lit_map.input(pos2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,                  olit);
  solver.add_clause( ilit1,                  olit);
  solver.add_clause( ilit2,                  olit);
  solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNANDN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogicNANDN::TpgLogicNANDN(ymuint id,
			     const char* name,
			     ymuint fanin_num,
			     TpgNode** fanin_array,
			     TpgFault** fault_array) :
  TpgLogicN(id, name, fanin_num, fanin_array, fault_array)
{
}

// @brief デストラクタ
TpgLogicNANDN::~TpgLogicNANDN()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNANDN::gate_type() const
{
  return kGateNAND;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNANDN::cval() const
{
  return kVal0;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNANDN::nval() const
{
  return kVal1;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNANDN::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNANDN::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNANDN::make_cnf(SatSolver& solver,
			const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  ymuint ni = fanin_num();
  vector<SatLiteral> tmp_lits(ni + 1);
  for (ymuint i = 0; i < ni; ++ i) {
    SatLiteral ilit = lit_map.input(i);
    solver.add_clause(ilit, olit);
    tmp_lits[i] = ~ilit;
  }
  tmp_lits[ni] = ~olit;
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
TpgLogicNANDN::make_faulty_cnf(SatSolver& solver,
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
    solver.add_clause(ilit, olit);
    tmp_lits.push_back(~ilit);
  }
  tmp_lits.push_back(~olit);
  solver.add_clause(tmp_lits);
}

END_NAMESPACE_YM_SATPG

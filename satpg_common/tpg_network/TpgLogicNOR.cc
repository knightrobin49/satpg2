
/// @file TpgLogicNOR.cc
/// @brief TpgLogicNOR[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicNOR.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNOR2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1 ファンインのノード
TpgLogicNOR2::TpgLogicNOR2(ymuint id,
			   const char* name,
			   TpgNode* inode0,
			   TpgNode* inode1) :
  TpgLogic2(id, name, inode0, inode1)
{
}

// @brief デストラクタ
TpgLogicNOR2::~TpgLogicNOR2()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNOR2::gate_type() const
{
  return kGateNOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR2::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR2::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR2::coval() const
{
  return kVal0;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR2::noval() const
{
  return kVal1;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNOR2::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,         ~olit);
  solver.add_clause(~ilit1,         ~olit);
  solver.add_clause( ilit0,  ilit1,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNOR2::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 0 );
  if ( fpos == 0 ) {
    SatLiteral ilit1 = lit_map.input(1);
    SatLiteral olit  = lit_map.output();
    solver.add_clause(~ilit1, ~olit);
    solver.add_clause( ilit1,  olit);
  }
  else {
    SatLiteral ilit0 = lit_map.input(0);
    SatLiteral olit  = lit_map.output();
    solver.add_clause(~ilit0, ~olit);
    solver.add_clause( ilit0,  olit);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNOR3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2 ファンインのノード
TpgLogicNOR3::TpgLogicNOR3(ymuint id,
			   const char* name,
			   TpgNode* inode0,
			   TpgNode* inode1,
			   TpgNode* inode2) :
  TpgLogic3(id, name, inode0, inode1, inode2)
{
}

// @brief デストラクタ
TpgLogicNOR3::~TpgLogicNOR3()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNOR3::gate_type() const
{
  return kGateNOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR3::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR3::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR3::coval() const
{
  return kVal0;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR3::noval() const
{
  return kVal1;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNOR3::make_cnf(SatSolver& solver,
		      const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,                 ~olit);
  solver.add_clause(~ilit1,                 ~olit);
  solver.add_clause(~ilit2,                 ~olit);
  solver.add_clause( ilit0,  ilit1,  ilit2,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNOR3::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 0 );
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  switch ( fpos ) {
  case 0:
    solver.add_clause(~ilit1,         ~olit);
    solver.add_clause(~ilit2,         ~olit);
    solver.add_clause( ilit1,  ilit2,  olit);
    break;
  case 1:
    solver.add_clause(~ilit0,         ~olit);
    solver.add_clause(~ilit2,         ~olit);
    solver.add_clause( ilit0,  ilit2,  olit);
    break;
  case 2:
    solver.add_clause(~ilit0,         ~olit);
    solver.add_clause(~ilit1,         ~olit);
    solver.add_clause( ilit0,  ilit1,  olit);
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNOR4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
TpgLogicNOR4::TpgLogicNOR4(ymuint id,
			   const char* name,
			   TpgNode* inode0,
			   TpgNode* inode1,
			   TpgNode* inode2,
			   TpgNode* inode3) :
  TpgLogic4(id, name, inode0, inode1, inode2, inode3)
{
}

// @brief デストラクタ
TpgLogicNOR4::~TpgLogicNOR4()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNOR4::gate_type() const
{
  return kGateNOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR4::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR4::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR4::coval() const
{
  return kVal0;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNOR4::noval() const
{
  return kVal1;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNOR4::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(3);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,                         ~olit);
  solver.add_clause(~ilit1,                         ~olit);
  solver.add_clause(~ilit2,                         ~olit);
  solver.add_clause(~ilit3,                         ~olit);
  solver.add_clause( ilit0,  ilit1,  ilit2,  ilit3,  olit);
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgLogicNOR4::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 0 );
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(3);
  SatLiteral olit  = lit_map.output();
  switch ( fpos ) {
  case 0:
    solver.add_clause(~ilit1,                 ~olit);
    solver.add_clause(~ilit2,                 ~olit);
    solver.add_clause(~ilit3,                 ~olit);
    solver.add_clause( ilit1,  ilit2,  ilit3,  olit);
    break;
  case 1:
    solver.add_clause(~ilit0,                 ~olit);
    solver.add_clause(~ilit2,                 ~olit);
    solver.add_clause(~ilit3,                 ~olit);
    solver.add_clause( ilit0,  ilit2,  ilit3,  olit);
    break;
  case 2:
    solver.add_clause(~ilit0,                 ~olit);
    solver.add_clause(~ilit1,                 ~olit);
    solver.add_clause(~ilit3,                 ~olit);
    solver.add_clause( ilit0,  ilit1,  ilit3,  olit);
    break;
  case 3:
    solver.add_clause(~ilit0,                 ~olit);
    solver.add_clause(~ilit1,                 ~olit);
    solver.add_clause(~ilit2,                 ~olit);
    solver.add_clause( ilit0,  ilit1,  ilit2,  olit);
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNORN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogicNORN::TpgLogicNORN(ymuint id,
			   const char* name,
			   ymuint fanin_num,
			   TpgNode** fanin_array,
			   TpgFault** fault_array) :
  TpgLogicN(id, name, fanin_num, fanin_array, fault_array)
{
}

// @brief デストラクタ
TpgLogicNORN::~TpgLogicNORN()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgLogicNORN::gate_type() const
{
  return kGateNOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNORN::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNORN::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNORN::coval() const
{
  return kVal0;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicNORN::noval() const
{
  return kVal1;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicNORN::make_cnf(SatSolver& solver,
		       const LitMap& lit_map) const
{
  SatLiteral olit  = lit_map.output();
  ymuint ni = fanin_num();
  vector<SatLiteral> tmp_lits(ni + 1);
  for (ymuint i = 0; i < ni; ++ i) {
    SatLiteral ilit = lit_map.input(i);
    solver.add_clause(~ilit, ~olit);
    tmp_lits[i] = ilit;
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
TpgLogicNORN::make_faulty_cnf(SatSolver& solver,
			      ymuint fpos,
			      int fval,
			      const LitMap& lit_map) const
{
  ASSERT_COND( fval == 0 );
  SatLiteral olit  = lit_map.output();
  ymuint ni = fanin_num();
  vector<SatLiteral> tmp_lits;
  tmp_lits.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    if ( i == fpos ) {
      continue;
    }
    SatLiteral ilit = lit_map.input(i);
    solver.add_clause(~ilit, ~olit);
    tmp_lits.push_back(ilit);
  }
  tmp_lits.push_back(olit);
  solver.add_clause(tmp_lits);
}

END_NAMESPACE_YM_SATPG

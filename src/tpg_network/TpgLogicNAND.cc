
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
// @param[in] inode0, inode1 ファンインのノード
TpgLogicNAND2::TpgLogicNAND2(ymuint id,
			     TpgNode* inode0,
			     TpgNode* inode1) :
  TpgLogic2(id, inode0, inode1)
{
}

// @brief デストラクタ
TpgLogicNAND2::~TpgLogicNAND2()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
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
			const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,          olit);
  solver.add_clause( ilit1,          olit);
  solver.add_clause(~ilit0, ~ilit1, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNAND3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] inode0, inode1, inode2 ファンインのノード
TpgLogicNAND3::TpgLogicNAND3(ymuint id,
			     TpgNode* inode0,
			     TpgNode* inode1,
			     TpgNode* inode2) :
  TpgLogic3(id, inode0, inode1, inode2)
{
}

// @brief デストラクタ
TpgLogicNAND3::~TpgLogicNAND3()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
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
			const LitMap& lit_map)
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


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNAND4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
TpgLogicNAND4::TpgLogicNAND4(ymuint id,
			     TpgNode* inode0,
			     TpgNode* inode1,
			     TpgNode* inode2,
			     TpgNode* inode3) :
  TpgLogic4(id, inode0, inode1, inode2, inode3)
{
}

// @brief デストラクタ
TpgLogicNAND4::~TpgLogicNAND4()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
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
			const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause( ilit0,                          olit);
  solver.add_clause( ilit1,                          olit);
  solver.add_clause( ilit2,                          olit);
  solver.add_clause( ilit3,                          olit);
  solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~ilit3, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicNANDN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogicNANDN::TpgLogicNANDN(ymuint id,
			     ymuint fanin_num,
			     TpgNode** fanin_array,
			     TpgFault** fault_array) :
  TpgLogicN(id, fanin_num, fanin_array, fault_array)
{
}

// @brief デストラクタ
TpgLogicNANDN::~TpgLogicNANDN()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
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
			const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(2);
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

END_NAMESPACE_YM_SATPG

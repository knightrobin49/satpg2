
/// @file TpgLogicOR.cc
/// @brief TpgLogicOR[x] の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicOR.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicOR2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1 ファンインのノード
TpgLogicOR2::TpgLogicOR2(ymuint id,
			 const char* name,
			 TpgNode* inode0,
			 TpgNode* inode1) :
  TpgLogic2(id, name, inode0, inode1)
{
}

// @brief デストラクタ
TpgLogicOR2::~TpgLogicOR2()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicOR2::gate_type() const
{
  return kGateOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR2::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR2::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR2::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR2::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicOR2::make_cnf(SatSolver& solver,
		      const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,          olit);
  solver.add_clause(~ilit1,          olit);
  solver.add_clause( ilit0,  ilit1, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicOR3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2 ファンインのノード
TpgLogicOR3::TpgLogicOR3(ymuint id,
			 const char* name,
			 TpgNode* inode0,
			 TpgNode* inode1,
			 TpgNode* inode2) :
  TpgLogic3(id, name, inode0, inode1, inode2)
{
}

// @brief デストラクタ
TpgLogicOR3::~TpgLogicOR3()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicOR3::gate_type() const
{
  return kGateOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR3::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR3::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR3::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR3::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicOR3::make_cnf(SatSolver& solver,
		      const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,                  olit);
  solver.add_clause(~ilit1,                  olit);
  solver.add_clause(~ilit2,                  olit);
  solver.add_clause( ilit0,  ilit1,  ilit2, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicOR4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
TpgLogicOR4::TpgLogicOR4(ymuint id,
			 const char* name,
			 TpgNode* inode0,
			 TpgNode* inode1,
			 TpgNode* inode2,
			 TpgNode* inode3) :
  TpgLogic4(id, name, inode0, inode1, inode2, inode3)
{
}

// @brief デストラクタ
TpgLogicOR4::~TpgLogicOR4()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicOR4::gate_type() const
{
  return kGateOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR4::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR4::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR4::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicOR4::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicOR4::make_cnf(SatSolver& solver,
		      const LitMap& lit_map)
{
  SatLiteral ilit0 = lit_map.input(0);
  SatLiteral ilit1 = lit_map.input(1);
  SatLiteral ilit2 = lit_map.input(2);
  SatLiteral ilit3 = lit_map.input(2);
  SatLiteral olit  = lit_map.output();
  solver.add_clause(~ilit0,                          olit);
  solver.add_clause(~ilit1,                          olit);
  solver.add_clause(~ilit2,                          olit);
  solver.add_clause(~ilit3,                          olit);
  solver.add_clause( ilit0,  ilit1,  ilit2,  ilit3, ~olit);
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicORN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogicORN::TpgLogicORN(ymuint id,
			 const char* name,
			 ymuint fanin_num,
			 TpgNode** fanin_array,
			 TpgFault** fault_array) :
  TpgLogicN(id, name, fanin_num, fanin_array, fault_array)
{
}

// @brief デストラクタ
TpgLogicORN::~TpgLogicORN()
{
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
TpgNode::GateType
TpgLogicORN::gate_type() const
{
  return kGateOR;
}

// @brief controling value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicORN::cval() const
{
  return kVal1;
}

// @brief noncontroling valueを得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicORN::nval() const
{
  return kVal0;
}

// @brief controling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicORN::coval() const
{
  return kVal1;
}

// @brief noncontroling output value を得る．
//
// is_logic() が false の場合の返り値は不定
// ない場合は kValX を返す．
Val3
TpgLogicORN::noval() const
{
  return kVal0;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgLogicORN::make_cnf(SatSolver& solver,
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
    solver.add_clause(~ilit, olit);
    tmp_lits[i] = ilit;
  }
  tmp_lits[ni] = ~olit;
  solver.add_clause(tmp_lits);
}

END_NAMESPACE_YM_SATPG

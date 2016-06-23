
/// @file TpgOutput.cc
/// @brief TpgOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOutput.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] output_id 出力番号
// @param[in] fanin ファンインのノード
TpgOutput::TpgOutput(ymuint id,
		     const char* name,
		     ymuint output_id,
		     TpgNode* fanin) :
  TpgNode(id, name),
  mOutputId(output_id),
  mFanin(fanin)
{
}

// @brief デストラクタ
TpgOutput::~TpgOutput()
{
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
bool
TpgOutput::is_output() const
{
  return true;
}

// @brief 外部出力タイプの時に出力番号を返す．
//
// node = TpgNetwork::output(node->output_id())
// の関係を満たす．
// is_output() が false の場合の返り値は不定
ymuint
TpgOutput::output_id() const
{
  return mOutputId;
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
ymuint
TpgOutput::output_id2() const
{
  return mOutputId2;
}

// @brief ゲートタイプを得る．
//
// is_logic() が false の場合の返り値は不定
GateType
TpgOutput::gate_type() const
{
  return kGateBUFF;
}

// @brief ファンイン数を得る．
ymuint
TpgOutput::fanin_num() const
{
  return 1;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgOutput::fanin(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mFanin;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgOutput::make_cnf(SatSolver& solver,
		    const LitMap& lit_map) const
{
  SatLiteral ilit = lit_map.input(0);
  SatLiteral olit = lit_map.output();
  solver.add_clause( ilit, ~olit);
  solver.add_clause(~ilit,  olit);
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgOutput::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  return mFaults[val];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgOutput::input_fault(int val,
		       ymuint pos)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  return mFaults[val];
}

// @brief 出力番号2をセットする．
// @param[in] id セットする番号
//
// 出力ノード以外では無効
void
TpgOutput::set_output_id2(ymuint id)
{
  mOutputId2 = id;
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgOutput::set_input_fault(int val,
			   ymuint pos,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  mFaults[val % 2] = fault;
}

END_NAMESPACE_YM_SATPG

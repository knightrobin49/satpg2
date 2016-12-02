
/// @file TpgOutput.cc
/// @brief TpgOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOutput.h"
#include "TpgDffInput.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] output_id 出力番号
// @param[in] fanin ファンインのノード
TpgOutput::TpgOutput(ymuint id,
		     ymuint output_id,
		     TpgNode* fanin) :
  TpgNode(id, vector<TpgNode*>(1, fanin), 0),
  mOutputId(output_id)
{
  mOutputId2 = 0;
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

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgOutput::make_faulty_cnf(SatSolver& solver,
			   ymuint fpos,
			   int fval,
			   const LitMap& lit_map) const
{
  SatLiteral olit = lit_map.output();
  if ( fval == 0 ) {
    solver.add_clause(~olit);
  }
  else {
    solver.add_clause( olit);
  }
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


//////////////////////////////////////////////////////////////////////
// クラス TpgDffInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] output_id 出力番号
// @param[in] fanin ファンインのノード
TpgDffInput::TpgDffInput(ymuint id,
			 ymuint output_id,
			 TpgNode* fanin) :
  TpgOutput(id, output_id, fanin)
{
}

// @brief デストラクタ
TpgDffInput::~TpgDffInput()
{
}

// @brief DFF の入力に接続している外部出力タイプの時 true を返す．
bool
TpgDffInput::is_dff_input() const
{
  return true;
}

// @brief DFF の入力に接続している外部出力タイプの時に対応する外部入力を返す．
//
// is_dff_input() == false の時には nullptr を返す．
TpgNode*
TpgDffInput::alt_input() const
{
  return mAltNode;
}

// @brief DFFの入出力の時に相方のノードを設定する．
void
TpgDffInput::set_alt_node(TpgNode* alt_node)
{
  mAltNode = alt_node;
}

END_NAMESPACE_YM_SATPG

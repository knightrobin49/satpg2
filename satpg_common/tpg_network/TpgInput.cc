
/// @file TpgInput.cc
/// @brief TpgInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgInput.h"
#include "TpgDffOutput.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] input_id 入力番号
// @param[in] fanout_num ファンアウト数
TpgInput::TpgInput(ymuint id,
		   ymuint input_id,
		   ymuint fanout_num) :
  TpgNode(id, vector<TpgNode*>(0), fanout_num),
  mInputId(input_id)
{
}

// @brief デストラクタ
TpgInput::~TpgInput()
{
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
bool
TpgInput::is_input() const
{
  return true;
}

// @brief 外部入力タイプの時に入力番号を返す．
//
// node = TpgNetwork::input(node->input_id()
// の関係を満たす．
// is_input() が false の場合の返り値は不定
ymuint
TpgInput::input_id() const
{
  return mInputId;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgInput::make_cnf(SatSolver& solver,
		   const LitMap& lit_map) const
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス TpgDffOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] input_id 入力番号
// @param[in] fanout_num ファンアウト数
TpgDffOutput::TpgDffOutput(ymuint id,
			   ymuint input_id,
			   ymuint fanout_num) :
  TpgInput(id, input_id, fanout_num)
{
}

// @brief デストラクタ
TpgDffOutput::~TpgDffOutput()
{
}

// @brief DFF の出力に接続している外部入力タイプの時 true を返す．
bool
TpgDffOutput::is_dff_output() const
{
  return true;
}

// @brief DFF の出力に接続している外部入力タイプの時に対応する外部出力を返す．
//
// is_dff_output() == false の時には nullptr を返す．
TpgNode*
TpgDffOutput::alt_output() const
{
  return mAltNode;
}

// @brief DFFの入出力の時に相方のノードを設定する．
void
TpgDffOutput::set_alt_node(TpgNode* alt_node)
{
  mAltNode = alt_node;
}

END_NAMESPACE_YM_SATPG

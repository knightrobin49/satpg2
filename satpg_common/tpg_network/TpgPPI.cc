
/// @file TpgPPI.cc
/// @brief TpgPPI の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgPPI.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgPPI
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] input_id 入力番号
// @param[in] fanout_num ファンアウト数
TpgPPI::TpgPPI(ymuint id,
	       ymuint input_id,
	       ymuint fanout_num) :
  TpgNode(id, vector<TpgNode*>(0), fanout_num),
  mInputId(input_id)
{
}

// @brief デストラクタ
TpgPPI::~TpgPPI()
{
}

// @brief 入力タイプの時 true を返す．
//
// 具体的には is_primary_input() || is_dff_output()
bool
TpgPPI::is_ppi() const
{
  return true;
}

// @brief 外部入力タイプの時に入力番号を返す．
//
// node = TpgNetwork::input(node->input_id()
// の関係を満たす．
// is_input() が false の場合の返り値は不定
ymuint
TpgPPI::input_id() const
{
  return mInputId;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgPPI::make_cnf(SatSolver& solver,
			 const LitMap& lit_map) const
{
  // なにもしない．
}

END_NAMESPACE_YM_SATPG

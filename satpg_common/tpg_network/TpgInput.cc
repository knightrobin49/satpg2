
/// @file TpgInput.cc
/// @brief TpgInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgInput.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] input_id 入力番号
TpgInput::TpgInput(ymuint id,
		   const char* name,
		   ymuint input_id) :
  TpgNode(id, name, 0, nullptr, nullptr),
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

END_NAMESPACE_YM_SATPG

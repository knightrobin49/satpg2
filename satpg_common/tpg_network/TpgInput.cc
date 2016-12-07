
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
// @param[in] input_id 入力番号
// @param[in] fanout_num ファンアウト数
TpgInput::TpgInput(ymuint id,
		   ymuint input_id,
		   ymuint fanout_num) :
  TpgPPI(id, input_id, fanout_num)
{
}

// @brief デストラクタ
TpgInput::~TpgInput()
{
}

// @brief 外部入力タイプの時 true を返す．
bool
TpgInput::is_primary_input() const
{
  return true;
}

END_NAMESPACE_YM_SATPG

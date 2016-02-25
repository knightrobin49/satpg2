
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
TpgInput::TpgInput(ymuint id,
		   ymuint input_id) :
  TpgNode(id),
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

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgInput::output_fault(int val) const
{
  ASSERT_COND( val == 0 || val == 1 );
  return mFaults[val % 2];
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgInput::fault_num() const
{
  return 2;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgInput::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaults[pos % 2];
}

END_NAMESPACE_YM_SATPG

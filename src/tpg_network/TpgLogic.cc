
/// @file TpgLogic.cc
/// @brief TpgLogic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
TpgLogic::TpgLogic(ymuint id) :
  TpgNode(id)
{
}

// @brief デストラクタ
TpgLogic::~TpgLogic()
{
}

// @brief logic タイプの時 true を返す．
bool
TpgLogic::is_logic() const
{
  return true;
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgLogic::output_fault(int val) const
{
  ASSERT_COND( val == 0 || val == 1 );
  return mOutputFaults[val % 2];
}

// @brief 出力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] fault 故障
void
TpgLogic::set_output_fault(int val,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  mOutputFaults[val % 2] = fault;
}

END_NAMESPACE_YM_SATPG

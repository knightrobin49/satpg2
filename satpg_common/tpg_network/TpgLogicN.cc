
/// @file TpgLogicN.cc
/// @brief TpgLogicN の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicN.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogicN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogicN::TpgLogicN(ymuint id,
		     const char* name,
		     ymuint fanin_num,
		     TpgNode** fanin_array,
		     TpgFault** fault_array) :
  TpgLogic(id, name),
  mFaninNum(fanin_num),
  mFaninArray(fanin_array),
  mInputFaults(fault_array)
{
}

// @brief デストラクタ
TpgLogicN::~TpgLogicN()
{
  // mFaninArray は TpgNetwork が管理している．
}

// @brief ファンイン数を得る．
ymuint
TpgLogicN::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogicN::fanin(ymuint pos) const
{
  ASSERT_COND( pos < fanin_num() );
  return mFaninArray[pos];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogicN::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < fanin_num() );
  return mInputFaults[((pos % fanin_num()) * 2) + (val % 2)];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgLogicN::input_fault(int val,
		       ymuint pos)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < fanin_num() );
  return mInputFaults[((pos % fanin_num()) * 2) + (val % 2)];
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgLogicN::set_input_fault(int val,
			   ymuint pos,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < fanin_num() );
  mInputFaults[((pos % fanin_num()) * 2) + (val % 2)] = fault;
}

END_NAMESPACE_YM_SATPG

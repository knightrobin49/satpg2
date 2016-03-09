
/// @file TpgLogic4.cc
/// @brief TpgLogic4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic4.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
TpgLogic4::TpgLogic4(ymuint id,
		     const char* name,
		     TpgNode* inode0,
		     TpgNode* inode1,
		     TpgNode* inode2,
		     TpgNode* inode3) :
  TpgLogic(id, name)
{
  mFanins[0] = inode0;
  mFanins[1] = inode1;
  mFanins[2] = inode2;
  mFanins[3] = inode3;
}

// @brief デストラクタ
TpgLogic4::~TpgLogic4()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic4::fanin_num() const
{
  return 4;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic4::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 4 );
  return mFanins[pos];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogic4::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 4 );
  return mInputFaults[((pos % 4) * 2) + (val % 2)];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgLogic4::input_fault(int val,
		       ymuint pos)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 4 );
  return mInputFaults[((pos % 4) * 2) + (val % 2)];
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgLogic4::set_input_fault(int val,
			   ymuint pos,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 4 );
  mInputFaults[((pos % 4) * 2) + (val % 2)] = fault;
}

END_NAMESPACE_YM_SATPG

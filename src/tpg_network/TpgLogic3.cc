
/// @file TpgLogic3.cc
/// @brief TpgLogic3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode0, inode1, inode2 ファンインのノード
TpgLogic3::TpgLogic3(ymuint id,
		     const char* name,
		     TpgNode* inode0,
		     TpgNode* inode1,
		     TpgNode* inode2) :
  TpgLogic(id, name)
{
  mFanins[0] = inode0;
  mFanins[1] = inode1;
  mFanins[2] = inode2;
}

// @brief デストラクタ
TpgLogic3::~TpgLogic3()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic3::fanin_num() const
{
  return 3;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic3::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 3 );
  return mFanins[pos];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogic3::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 3 );
  return mInputFaults[((pos % 3) * 2) + (val % 2)];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgLogic3::input_fault(int val,
		       ymuint pos)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 3 );
  return mInputFaults[((pos % 3) * 2) + (val % 2)];
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgLogic3::set_input_fault(int val,
			   ymuint pos,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 3 );
  mInputFaults[((pos % 3) * 2) + (val % 2)] = fault;
}

END_NAMESPACE_YM_SATPG

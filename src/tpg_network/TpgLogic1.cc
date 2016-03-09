
/// @file TpgLogic1.cc
/// @brief TpgLogic1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic1.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] inode ファンインのノード
TpgLogic1::TpgLogic1(ymuint id,
		     const char* name,
		     TpgNode* inode) :
  TpgLogic(id, name),
  mFanin(inode)
{
}

// @brief デストラクタ
TpgLogic1::~TpgLogic1()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic1::fanin_num() const
{
  return 1;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic1::fanin(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mFanin;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogic1::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  return mInputFaults[val % 2];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgLogic1::input_fault(int val,
		       ymuint pos)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  return mInputFaults[val % 2];
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgLogic1::set_input_fault(int val,
			   ymuint pos,
			   TpgFault* fault)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  mInputFaults[val % 2] = fault;
}

END_NAMESPACE_YM_SATPG

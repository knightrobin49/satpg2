
/// @file TpgLogic2.cc
/// @brief TpgLogic2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] inode0, inode1 ファンインのノード
TpgLogic2::TpgLogic2(ymuint id,
		     TpgNode* inode0,
		     TpgNode* inode1) :
  TpgLogic(id)
{
  mFanins[0] = inode0;
  mFanins[1] = inode1;
}

// @brief デストラクタ
TpgLogic2::~TpgLogic2()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic2::fanin_num() const
{
  return 2;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic2::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 2 );
  return mFanins[pos];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogic2::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < 2 );
  return mInputFaults[(pos * 2) + val];
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgLogic2::fault_num() const
{
  return 6;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgLogic2::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  ymuint p1 = pos / 2;
  ymuint val = pos % 2;
  if ( p1 == 0 ) {
    return output_fault(val);
  }
  else {
    return input_fault(p1 - 1, val);
  }
}

END_NAMESPACE_YM_SATPG


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
// @param[in] fanin_num ファンイン数
// @param[in] fanin_array ファンインの配列
TpgLogicN::TpgLogicN(ymuint id,
		     ymuint fanin_num,
		     TpgNode** fanin_array) :
  TpgLogic(id),
  mFaninNum(fanin_num),
  mFaninArray(fanin_array)
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
  return mInputFaults[(pos * 2) + val];
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgLogicN::fault_num() const
{
  return fanin_num() * 2 + 2;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgLogicN::fault(ymuint pos) const
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

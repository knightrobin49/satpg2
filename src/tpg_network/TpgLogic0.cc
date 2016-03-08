
/// @file TpgLogic0.cc
/// @brief TpgLogic0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic0.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
TpgLogic0::TpgLogic0(ymuint id,
		     const char* name) :
  TpgLogic(id, name)
{
}

// @brief デストラクタ
TpgLogic0::~TpgLogic0()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic0::fanin_num() const
{
  return 0;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic0::fanin(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgLogic0::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgLogic0::fault_num() const
{
  return 2;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgLogic0::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return output_fault(pos);
}

END_NAMESPACE_YM_SATPG

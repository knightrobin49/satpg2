
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
// @param[in] name 名前
// @param[in] fanin_array ファンインの配列
// @param[in] fault_array 入力の故障の配列
TpgLogic::TpgLogic(ymuint id,
		   const char* name,
		   ymuint fanin_num,
		   TpgNode** fanin_array,
		   TpgFault** fault_array) :
  TpgNode(id, name, fanin_num, fanin_array, fault_array)
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

END_NAMESPACE_YM_SATPG

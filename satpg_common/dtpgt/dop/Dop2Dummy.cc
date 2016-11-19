
/// @file Dop2Dummy.cc
/// @brief Dop2Dummy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2Dummy.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
DetectOp2*
new_Dop2Dummy()
{
  return new Dop2Dummy();
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2Dummy
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Dop2Dummy::Dop2Dummy()
{
}

// @brief デストラクタ
Dop2Dummy::~Dop2Dummy()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値の割当リスト
void
Dop2Dummy::operator()(const TpgFault* f,
		      const NodeVal2List& assign_list)
{
}

END_NAMESPACE_YM_SATPG


/// @file UopDummy.cc
/// @brief UopDummy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "UopDummy.h"


BEGIN_NAMESPACE_YM_SATPG_TD

// @brief 'dummy' タイプを生成する．
UntestOp*
new_UopDummy()
{
  return new UopDummy();
}


//////////////////////////////////////////////////////////////////////
// クラス UopDummy
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UopDummy::UopDummy()
{
}

// @brief デストラクタ
UopDummy::~UopDummy()
{
}

// @brief テスト不能故障と判定された時の処理
// @param[in] f 故障
void
UopDummy::operator()(const TpgFault* f)
{
}

END_NAMESPACE_YM_SATPG_TD

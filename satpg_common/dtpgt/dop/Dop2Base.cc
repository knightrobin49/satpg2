
/// @file Dop2Base.cc
/// @brief Dop2Base の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2Base.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] fmgr FaultMgr
DetectOp2*
new_Dop2Base(FaultMgr& fmgr)
{
  return new Dop2Base(fmgr);
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2Base
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr FaultMgr
Dop2Base::Dop2Base(FaultMgr& fmgr) :
  mMgr(fmgr)
{
}

// @brief デストラクタ
Dop2Base::~Dop2Base()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
Dop2Base::operator()(const TpgFault* f,
		     const NodeVal2List& assign_list)

{
  mMgr.set_status(f, kFsDetected);
}

END_NAMESPACE_YM_SATPG

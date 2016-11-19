
/// @file Dop2Drop.cc
/// @brief Dop2Drop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2Drop.h"
#include "Fsim.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'drop' タイプを生成する．
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DetectOp2*
new_Dop2Drop(FaultMgr& fmgr,
	     Fsim& fsim)
{
  return new Dop2Drop(fmgr, fsim);
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2Drop
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
Dop2Drop::Dop2Drop(FaultMgr& fmgr,
		   Fsim& fsim) :
  mMgr(fmgr),
  mFsim(fsim),
  mOp(fmgr, fsim)
{
}

// @brief デストラクタ
Dop2Drop::~Dop2Drop()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
Dop2Drop::operator()(const TpgFault* f,
		     const NodeVal2List& assign_list)
{
  mFsim.sppfp(assign_list, mOp);
}

END_NAMESPACE_YM_SATPG


/// @file FopRtpg.cc
/// @brief FopRtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "FopRtpg.h"
#include "Fsim.h"
#include "TpgFaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG_TD

//////////////////////////////////////////////////////////////////////
// クラス FopRtpg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] fmgr 故障マネージャ
FopRtpg::FopRtpg(Fsim& fsim,
		 TpgFaultMgr& fmgr) :
  mFsim(fsim),
  mFaultMgr(fmgr)
{
  clear_count();
}

// @brief デストラクタ
FopRtpg::~FopRtpg()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
FopRtpg::operator()(const TpgFault* f,
		    PackedVal dpat)
{
  mFsim.set_skip(f);
  mFaultMgr.set_status(f, kFsDetected);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      ++ mCount[i];
      // 最初に検出したビットの回数のみインクリメントする．
      break;
    }
  }
}

// @brief 初期化する．
void
FopRtpg::init()
{
  clear_count();
}

// @brief 検出回数をクリアする．
void
FopRtpg::clear_count()
{
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mCount[i] = 0;
  }
}

// @brief 検出回数を得る．
// @param[in] bitpos ビット位置
ymuint
FopRtpg::count(ymuint bitpos)
{
  return mCount[bitpos];
}

END_NAMESPACE_YM_SATPG_TD

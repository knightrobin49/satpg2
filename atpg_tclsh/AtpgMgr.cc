
/// @file AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "TpgFaultMgr.h"

#include "sa/TvMgr.h"
#include "sa/Fsim.h"

#include "td/TvMgr.h"
#include "td/Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE, TM_MISC)
{
  mFaultMgr = nullptr;
  mSaTvMgr = new nsSa::TvMgr();
#if 1
  mSaFsim = nsSa::new_Fsim2();
#else
  mSaFsim = nsSa::new_Fsim2New();
#endif
  mSaFsim3 = nsSa::new_Fsim3();

  mTdTvMgr = new nsTd::TvMgr();
  mTdFsim = nsTd::new_Fsim2();
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFaultMgr;
  delete mSaTvMgr;
  delete mSaFsim;
  delete mSaFsim3;

  delete mTdTvMgr;
  delete mTdFsim;
}

// @brief ファイル読み込みに関わる時間を得る．
USTime
AtpgMgr::read_time() const
{
  return mTimer.time(TM_READ);
}

// @brief DTPG に関わる時間を得る．
USTime
AtpgMgr::dtpg_time() const
{
  return mTimer.time(TM_DTPG);
}

// @brief 故障シミュレーションに関わる時間を得る．
USTime
AtpgMgr::fsim_time() const
{
  return mTimer.time(TM_FSIM);
}

// @brief SAT に関わる時間を得る．
USTime
AtpgMgr::sat_time() const
{
  return mTimer.time(TM_SAT);
}

// @brief その他の時間を得る．
USTime
AtpgMgr::misc_time() const
{
  return mTimer.time(TM_MISC);
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  delete mFaultMgr;
  mFaultMgr = new TpgFaultMgr(_network());

  mSaTvMgr->clear();
  mSaTvMgr->init(mNetwork.ppi_num());
  mSaFsim->set_network(mNetwork);
  mSaFsim3->set_network(mNetwork);

  //mTdTvMgr->clear();
  //mTdTvMgr->init(mNetwork.input_num(), mNetwork.dff_num());
  //mTdFsim->set_network(mNetwork);
}

END_NAMESPACE_YM_SATPG

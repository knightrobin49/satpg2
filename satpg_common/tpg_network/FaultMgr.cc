
/// @file FaultMgr.cc
/// @brief FaultMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012-2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FaultMgr.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
// @param[in] network 対象のネットワーク
FaultMgr::FaultMgr(const TpgNetwork& network) :
  mMaxFaultId(network.max_fault_id()),
  mFaultArray(mMaxFaultId, nullptr),
  mStatusArray(mMaxFaultId, kFsUndetected)
{
  ymuint n = network.rep_fault_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgFault* f = network.rep_fault(i);
    mFaultArray[f->id()] = f;
  }
}

// @brief デストラクタ
FaultMgr::~FaultMgr()
{
}

// @brief クリアする．
void
FaultMgr::clear_status()
{
  for (ymuint i = 0; i < mStatusArray.size(); ++ i) {
    mStatusArray[i] = kFsUndetected;
  }
}

// @brief 故障IDの最大値+1を返す．
ymuint
FaultMgr::max_fault_id() const
{
  return mMaxFaultId;
}

// @brief 故障IDから故障を返す．
// @param[in] id 故障ID
const TpgFault*
FaultMgr::fault(ymuint id) const
{
  ASSERT_COND( id < mMaxFaultId );
  return mFaultArray[id];
}

// @brief 故障の状態を得る．
FaultStatus
FaultMgr::status(const TpgFault* fault) const
{
  ASSERT_COND( fault->id() < mMaxFaultId );
  return mStatusArray[fault->id()];
}

// @brief fault の状態を変更する．
void
FaultMgr::set_status(const TpgFault* fault,
		     FaultStatus stat)
{
  ASSERT_COND( fault->id() < mStatusArray.size() );
  mStatusArray[fault->id()] = stat;
}

END_NAMESPACE_YM_SATPG

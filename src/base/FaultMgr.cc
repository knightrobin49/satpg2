
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
FaultMgr::FaultMgr()
{
  mChanged = false;
}

// @brief デストラクタ
FaultMgr::~FaultMgr()
{
}

// @brief クリアする．
void
FaultMgr::clear()
{
  mStatusArray.clear();

  mDetList.clear();
  mRemainList.clear();
  mUntestList.clear();

  mChanged = true;
}

// @brief network の故障を設定する．
void
FaultMgr::set_faults(const TpgNetwork& network)
{
  clear();

  ymuint max_id = network.max_fault_id();
  mStatusArray.resize(max_id, kFsUndetected);

  // 各ノードの故障を mRemainList に入れる．
  // ただし外部出力に到達可能でない故障は mUntestList に入れる．
  ymuint nn = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);
    ymuint nf = node->fault_num();
    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* fault = node->fault(j);
      mRepList.push_back(fault);
      if ( fault->tpg_onode()->is_active() ) {
	mRemainList.push_back(fault);
      }
      else {
	mUntestList.push_back(fault);
	set_status(fault, kFsUntestable);
      }
    }
  }
}

// @brief fault の状態を変更する．
void
FaultMgr::set_status(const TpgFault* fault,
		     FaultStatus stat)
{
  ASSERT_COND( fault->id() < mStatusArray.size() );
  mStatusArray[fault->id()] = stat;
  mChanged = true;
}

// @brief 故障リストをスキャンして未検出リストを更新する．
void
FaultMgr::update() const
{
  if ( mChanged ) {
    ymuint n = mRemainList.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < n; ++ rpos) {
      const TpgFault* f = mRemainList[rpos];
      switch ( status(f) ) {
      case kFsUndetected:
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;

      case kFsDetected:
	mDetList.push_back(f);
	break;

      case kFsUntestable:
	mUntestList.push_back(f);
	break;

      case kFsAborted:
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;
      }
    }
    if ( wpos < n ) {
      mRemainList.erase(mRemainList.begin() + wpos, mRemainList.end());
    }
    mChanged = false;
  }
}

END_NAMESPACE_YM_SATPG

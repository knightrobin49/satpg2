
/// @file SimFFR.cc
/// @brief SimFFR の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SimFFR.h"
#include "sa/FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

BEGIN_NONAMESPACE

void
prop2_recur(SimNode* node,
	    PackedVal oobs)
{
  node->_calc_gobs2(oobs);
  ymuint n = node->fanin_num();
  for (ymuint i = 0; i < n; ++ i) {
    SimNode* inode = node->fanin(i);
    if ( inode->is_ffr_root() ) {
      continue;
    }
    PackedVal lobs = node->lobs(i);
    prop2_recur(inode, lobs);
  }
}
END_NONAMESPACE

#if 1
// @brief FFR内の故障シミュレーションを行う．
PackedVal
SimFFR::fault_prop2()
{
  prop2_recur(root(), kPvAll1);

  PackedVal ffr_req = kPvAll0;
  for (vector<SimFault*>::const_iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip ) {
      continue;
    }

#if 0
    // ff の故障伝搬を行う．
    PackedVal lobs = kPvAll1;
    SimNode* simnode = ff->mNode;
    for (SimNode* node = simnode; !node->is_ffr_root(); ) {
      SimNode* onode = node->fanout(0);
      ymuint pos = node->fanout_ipos();
      lobs &= onode->_calc_gobs2(pos);
      node = onode;
    }

    PackedVal valdiff = ff->mInode->gval();
    const TpgFault* f = ff->mOrigF;
    if ( f->is_branch_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->_calc_gobs2(ipos);
    }
    if ( f->val() == 1 ) {
      valdiff = ~valdiff;
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
#else
    SimNode* simnode = ff->mNode;
    PackedVal valdiff = ff->mInode->gval();
    const TpgFault* f = ff->mOrigF;
    PackedVal lobs = kPvAll1;
    if ( f->is_branch_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs = simnode->lobs(ipos);
    }
    if ( f->val() == 1 ) {
      valdiff = ~valdiff;
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
#endif
  }

  return ffr_req;
}
#endif

#if 0
// @brief 検出可能な故障を見つける．
// @param[in] op 故障検出時に起動されるファンクタ
// @param[in] mask マスク
void
SimFFR::fault_sweep(FsimOp& op,
		    PackedVal mask)
{
  for (vector<SimFault*>::const_iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip ) {
      continue;
    }
    PackedVal pat = ff->mObsMask & mask;
    if ( pat != kPvAll0 ) {
      const TpgFault* f = ff->mOrigF;
      op(f, pat);
    }
  }
}
#endif

END_NAMESPACE_YM_SATPG_FSIM

﻿#ifndef SIMFFR_H
#define SIMFFR_H

/// @file SimFFR.h
/// @brief SimFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

class SimNode;
class SimFault;

//////////////////////////////////////////////////////////////////////
/// @class SimFFR SimFFR.h "SimFFR.h"
/// @brief Fanout-Free-Region を表すクラス
//////////////////////////////////////////////////////////////////////
class SimFFR
{
public:

  /// @brief コンストラクタ
  SimFFR();

  /// @brief デストラクタ
  ~SimFFR();


public:

  /// @brief 根のノードをセットする．
  void
  set_root(SimNode* root);

  /// @brief 根のノードを得る．
  SimNode*
  root() const;

  /// @brief このFFRの故障リストをクリアする．
  void
  clear_fault_list();

  /// @brief このFFRの故障リストに故障を追加する．
  void
  add_fault(SimFault* f);

  /// @brief FFR内の故障シミュレーションを行う．
  PackedVal
  fault_prop2();

  /// @brief 検出可能な故障を見つける．
  /// @param[out] fault_list 検出された故障を格納するリスト
  void
  fault_sweep(vector<const TpgFault*>& fault_list);

  /// @brief 検出可能な故障を見つける．
  /// @param[in] num テストベクタの数
  /// @param[in] mask マスク
  /// @param[out] fault_list 検出された故障とその時のビットパタンのリスト
  void
  fault_sweep(ymuint num,
	      PackedVal mask,
	      vector<pair<const TpgFault*, PackedVal> >& fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  SimNode* mRoot;

  // この FFR に属する故障のリスト
  vector<SimFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SimFFR::SimFFR()
{
}

// @brief デストラクタ
inline
SimFFR::~SimFFR()
{
}

// @brief 根のノードをセットする．
inline
void
SimFFR::set_root(SimNode* root)
{
  mRoot = root;
}

// @brief 根のノードを得る．
inline
SimNode*
SimFFR::root() const
{
  return mRoot;
}

// @brief このFFRの故障リストをクリアする．
inline
void
SimFFR::clear_fault_list()
{
  mFaultList.clear();
}

// @brief このFFRの故障リストに故障を追加する．
inline
void
SimFFR::add_fault(SimFault* f)
{
  mFaultList.push_back(f);
}

// @brief FFR内の故障シミュレーションを行う．
inline
PackedVal
SimFFR::fault_prop2()
{
  PackedVal ffr_req = kPvAll0;
  for (vector<SimFault*>::const_iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip ) {
      continue;
    }

    // ff の故障伝搬を行う．
    PackedVal lobs = kPvAll1;
    SimNode* simnode = ff->mNode;
    for (SimNode* node = simnode; !node->is_ffr_root(); ) {
      SimNode* onode = node->fanout(0);
      ymuint pos = node->fanout_ipos();
      lobs &= onode->gval()->_calc_gobs(pos);
      node = onode;
    }

    PackedVal valdiff = ff->mInode->gval()->val();
    const TpgFault* f = ff->mOrigF;
    if ( f->is_branch_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->gval()->_calc_gobs(ipos);
    }
    if ( f->val() == 1 ) {
      valdiff = ~valdiff;
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
  }

  return ffr_req;
}

// @brief 検出可能な故障を見つける．
// @param[out] fault_list 検出された故障を格納するリスト
inline
void
SimFFR::fault_sweep(vector<const TpgFault*>& fault_list)
{
  for (vector<SimFault*>::const_iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip || ff->mObsMask == kPvAll0 ) {
      continue;
    }
    const TpgFault* f = ff->mOrigF;
    fault_list.push_back(f);
  }
}

// @brief 検出可能な故障を見つける．
// @param[in] num テストベクタの数
// @param[in] mask マスク
// @param[out] fault_list 検出された故障とその時のビットパタンのリスト
void
SimFFR::fault_sweep(ymuint num,
		    PackedVal mask,
		    vector<pair<const TpgFault*, PackedVal> >& fault_list)
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
      fault_list.push_back(make_pair(f, pat));
    }
  }
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // SIMFFR_H

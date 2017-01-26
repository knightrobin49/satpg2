#ifndef SIMFFR_H
#define SIMFFR_H

/// @file SimFFR.h
/// @brief SimFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

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
  fault_prop() const;

  /// @brief このFFRに属する故障リストを得る．
  const vector<SimFault*>&
  fault_list() const;


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
SimFFR::fault_prop() const
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
      SimNode* onode = node->fanout_top();
      ymuint pos = node->fanout_ipos();
      lobs &= onode->_calc_gobs(pos);
      node = onode;
    }

    PackedVal valdiff = ff->mInode->gval();
    const TpgFault* f = ff->mOrigF;
    if ( f->is_branch_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->_calc_gobs(ipos);
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

// @brief このFFRに属する故障リストを得る．
inline
const vector<SimFault*>&
SimFFR::fault_list() const
{
  return mFaultList;
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // SIMFFR_H

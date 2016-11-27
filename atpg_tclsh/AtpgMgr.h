#ifndef ATPGMGR_H
#define ATPGMGR_H

/// @file AtpgMgr.h
/// @brief AtpgMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "sa/satpg_sa.h"
#include "td/satpg_td.h"

#include "TpgNetwork.h"
#include "ym/ym_cell.h"
#include "ym/Binder.h"
#include "ym/StopWatch.h"
#include "ym/MStopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class AtpgMgr AtpgMgr.h "AtpgMgr.h"
/// @brief ATPG に共通のデータを保持しておくためのクラス
//////////////////////////////////////////////////////////////////////
class AtpgMgr
{
public:

  /// @brief コンストラクタ
  AtpgMgr();

  /// @brief デストラクタ
  ~AtpgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief TgNetwork を取り出す．
  TpgNetwork&
  _network();

  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  nsSa::TvMgr&
  _sa_tv_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<nsSa::TestVector*>&
  _sa_tv_list();

  /// @brief 2値の故障シミュレータを取り出す．
  nsSa::Fsim&
  _sa_fsim();

  /// @brief 3値の故障シミュレータを返す．
  nsSa::Fsim&
  _sa_fsim3();

  /// @brief TvMgr を取り出す．
  nsTd::TvMgr&
  _td_tv_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<nsTd::TestVector*>&
  _td_tv_list();

  /// @brief 2値の故障シミュレータを取り出す．(遷移故障用)
  nsTd::Fsim&
  _td_fsim();


public:
  //////////////////////////////////////////////////////////////////////
  // ネットワーク設定に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークが変更された時に呼ばれる関数
  void
  after_set_network();


public:
  //////////////////////////////////////////////////////////////////////
  // イベントハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの変更に関するハンドラを登録する．
  void
  reg_network_handler(T2Binder<const TpgNetwork&, FaultMgr&>* handler);


public:
  //////////////////////////////////////////////////////////////////////
  // タイマー関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル読み込みに関わる時間を得る．
  USTime
  read_time() const;

  /// @brief DTPG に関わる時間を得る．
  USTime
  dtpg_time() const;

  /// @brief 故障シミュレーションに関わる時間を得る．
  USTime
  fsim_time() const;

  /// @brief SAT に関わる時間を得る．
  USTime
  sat_time() const;

  /// @brief その他の時間を得る．
  USTime
  misc_time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  TpgNetwork mNetwork;

  // 故障マネージャ
  FaultMgr* mFaultMgr;

  // テストベクタを管理するオブジェクト
  nsSa::TvMgr* mSaTvMgr;

  // テストベクタのリスト
  vector<nsSa::TestVector*> mSaTvList;

  // 故障シミュレータ
  nsSa::Fsim* mSaFsim;

  // 3値の故障シミュレータ
  nsSa::Fsim* mSaFsim3;

  // テストベクタを管理するオブジェクト
  nsTd::TvMgr* mTdTvMgr;

  // テストベクタのリスト
  vector<nsTd::TestVector*> mTdTvList;

  // 遷移故障用の故障シミュレータ
  nsTd::Fsim* mTdFsim;

  // ネットワークが変更された時に呼ばれるイベントハンドラ
  T2BindMgr<const TpgNetwork&, FaultMgr&> mNtwkBindMgr;

  // タイマー
  MStopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief TgNetwork を取り出す．
inline
TpgNetwork&
AtpgMgr::_network()
{
  return mNetwork;
}

// @brief FaultMgr を取り出す．
inline
FaultMgr&
AtpgMgr::_fault_mgr()
{
  return *mFaultMgr;
}

// @brief TvMgr を取り出す．
inline
nsSa::TvMgr&
AtpgMgr::_sa_tv_mgr()
{
  return *mSaTvMgr;
}

// @brief テストベクタのリストを取り出す．
inline
vector<nsSa::TestVector*>&
AtpgMgr::_sa_tv_list()
{
  return mSaTvList;
}

// @brief 2値の故障シミュレータを取り出す．
inline
nsSa::Fsim&
AtpgMgr::_sa_fsim()
{
  return *mSaFsim;
}

// @brief 3値の故障シミュレータを返す．
inline
nsSa::Fsim&
AtpgMgr::_sa_fsim3()
{
  return *mSaFsim3;
}

// @brief TvMgr を取り出す．
inline
nsTd::TvMgr&
AtpgMgr::_td_tv_mgr()
{
  return *mTdTvMgr;
}

// @brief テストベクタのリストを取り出す．
inline
vector<nsTd::TestVector*>&
AtpgMgr::_td_tv_list()
{
  return mTdTvList;
}

// @brief 2値の故障シミュレータを取り出す．(遷移故障用)
inline
nsTd::Fsim&
AtpgMgr::_td_fsim()
{
  return *mTdFsim;
}

// @brief ネットワークの変更に関するハンドラを登録する．
inline
void
AtpgMgr::reg_network_handler(T2Binder<const TpgNetwork&, FaultMgr&>* handler)
{
  mNtwkBindMgr.reg_binder(handler);
}

END_NAMESPACE_YM_SATPG

#endif // ATPGMGR_H

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
  TvMgr&
  _tv_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<TestVector*>&
  _tv_list();

  /// @brief Tv2Mgr を取り出す．
  Tv2Mgr&
  _tv2_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<TestVector2*>&
  _tv2_list();

  /// @brief 2値の故障シミュレータを取り出す．
  Fsim&
  _fsim();

  /// @brief 2値の故障シミュレータを取り出す．(遷移故障用)
  FsimT&
  _fsimt();

  /// @brief 3値の故障シミュレータを返す．
  Fsim&
  _fsim3();


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
  TvMgr* mTvMgr;

  // テストベクタのリスト
  vector<TestVector*> mTvList;

  // テストベクタを管理するオブジェクト
  Tv2Mgr* mTv2Mgr;

  // テストベクタのリスト
  vector<TestVector2*> mTv2List;

  // 故障シミュレータ
  Fsim* mFsim;

  // 遷移故障用の故障シミュレータ
  FsimT* mFsimT;

  // 3値の故障シミュレータ
  Fsim* mFsim3;

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
TvMgr&
AtpgMgr::_tv_mgr()
{
  return *mTvMgr;
}

// @brief テストベクタのリストを取り出す．
inline
vector<TestVector*>&
AtpgMgr::_tv_list()
{
  return mTvList;
}

// @brief TvMgr を取り出す．
inline
Tv2Mgr&
AtpgMgr::_tv2_mgr()
{
  return *mTv2Mgr;
}

// @brief テストベクタのリストを取り出す．
inline
vector<TestVector2*>&
AtpgMgr::_tv2_list()
{
  return mTv2List;
}

// @brief 2値の故障シミュレータを取り出す．
inline
Fsim&
AtpgMgr::_fsim()
{
  return *mFsim;
}

// @brief 2値の故障シミュレータを取り出す．(遷移故障用)
inline
FsimT&
AtpgMgr::_fsimt()
{
  return *mFsimT;
}

// @brief 3値の故障シミュレータを返す．
inline
Fsim&
AtpgMgr::_fsim3()
{
  return *mFsim3;
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

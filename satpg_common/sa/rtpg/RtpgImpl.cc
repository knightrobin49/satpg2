
/// @file RtpgImpl.cc
/// @brief Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgImpl.h"
#include "FaultMgr.h"
#include "sa/TvMgr.h"
#include "sa/TestVector.h"
#include "sa/Fsim.h"
#include "sa/RtpgStats.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_SA

Rtpg*
new_Rtpg()
{
  return new RtpgImpl();
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgImpl::RtpgImpl()
{
}

// @brief デストラクタ
RtpgImpl::~RtpgImpl()
{
}

// @brief 乱数生成器を初期化する．
// @param[in] seed 乱数の種
void
RtpgImpl::init(ymuint32 seed)
{
  mRandGen.init(seed);
}

// @brief RTPGを行なう．
// @param[in] fmgr 故障マネージャ
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[out] det_fault_list 検出された故障のリスト
// @param[out] tvlist テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
RtpgImpl::run(FaultMgr& fmgr,
	      TvMgr& tvmgr,
	      Fsim& fsim,
	      ymuint min_f,
	      ymuint max_i,
	      ymuint max_pat,
	      vector<const TestVector*>& tvlist,
	      RtpgStats& stats)
{
  StopWatch local_timer;

  local_timer.start();

  ymuint fnum = 0;
  ymuint undet_i = 0;
  ymuint epat_num = 0;
  ymuint total_det_count = 0;

  fsim.set_skip_all();
  for (ymuint i = 0; i < fmgr.max_fault_id(); ++ i) {
    const TpgFault* f = fmgr.fault(i);
    if ( f != nullptr && fmgr.status(f) == kFsUndetected ) {
      fsim.clear_skip(f);
      ++ fnum;
    }
  }

  TestVector* tv_array[kPvBitLen];
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tv_array[i] = tvmgr.new_vector();
  }

  const TestVector* cur_array[kPvBitLen];

  ymuint cpos = 0;
  ymuint pat_num = 0;
  for ( ; ; ) {
    if ( pat_num < max_pat ) {
      TestVector* tv = tv_array[cpos];
      tv->set_from_random(mRandGen);
      cur_array[cpos] = tv;
      ++ cpos;
      ++ pat_num;
      if ( cpos < kPvBitLen ) {
	continue;
      }
    }
    else if ( cpos == 0 ) {
      break;
    }

    vector<pair<const TpgFault*, PackedVal> > det_list;
    fsim.ppsfp(cpos, cur_array, det_list);

    ymuint det_count = det_list.size();
    bool det_flags[kPvBitLen];
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      det_flags[i] = false;
    }
    for (ymuint i = 0; i < det_count; ++ i) {
      const TpgFault* f = det_list[i].first;
      fmgr.set_status(f, kFsDetected);
      fsim.set_skip(f);
      PackedVal dpat = det_list[i].second;
      // dpat の最初の1のビットを調べる．
      ymuint first = 0;
      for ( ; first < cpos; ++ first) {
	if ( dpat & (1ULL << first) ) {
	  break;
	}
      }
      ASSERT_COND( first < cpos );
      det_flags[first] = true;
    }
    for (ymuint i = 0; i < cpos; ++ i) {
      if ( det_flags[i] ) {
	const TestVector* tv = cur_array[i];
	tvlist.push_back(tv);
	tv_array[i] = tvmgr.new_vector();
	++ epat_num;
      }
    }
    cpos = 0;

    total_det_count += det_count;

    if ( total_det_count == fnum ) {
      // すべての故障を検出した．
      break;
    }
    if ( det_count < min_f ) {
      // 検出故障数の下限を下回った
      break;
    }
    if ( det_count > 0 ) {
      undet_i = 0;
    }
    else {
      ++ undet_i;
      if ( undet_i > max_i ) {
	// 未検出の回数が max_i を越えた．
	break;
      }
    }
  }

  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tvmgr.delete_vector(tv_array[i]);
  }

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(total_det_count, pat_num, epat_num, time);
}

END_NAMESPACE_YM_SATPG_SA

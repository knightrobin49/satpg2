
/// @file RtpgP1.cc
/// @brief Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgP1.h"
#include "TpgFaultMgr.h"
#include "td/TvMgr.h"
#include "td/TestVector.h"
#include "td/Fsim.h"
#include "td/RtpgStats.h"
#include "FopRtpg.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_TD

Rtpg*
new_RtpgP1()
{
  RtpgP1* rtpg = new RtpgP1();
  return rtpg;
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgP1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgP1::RtpgP1()
{
}

// @brief デストラクタ
RtpgP1::~RtpgP1()
{
}

// @brief 乱数生成器を初期化する．
// @param[in] seed 乱数の種
void
RtpgP1::init(ymuint32 seed)
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
RtpgP1::run(TpgFaultMgr& fmgr,
	    TvMgr& tvmgr,
	    Fsim& fsim,
	    ymuint min_f,
	    ymuint max_i,
	    ymuint max_pat,
	    ymuint wsa_limit,
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
    if ( fmgr.status(f) == kFsUndetected ) {
      fsim.clear_skip(f);
      ++ fnum;
    }
  }

  TestVector* tv = tvmgr.new_vector();

  FopRtpg op(fsim, fmgr);

  op.init();

  ymuint gnum = 0;
  ymuint pat_num = 0;
  while ( pat_num < max_pat ) {
    tv->set_from_random(mRandGen);
    ++ gnum;

    ymuint wsa = fsim.calc_wsa(tv);
    //cout << "wsa = " << wsa << ", # of pat = " << pat_num << endl;
    if ( wsa > wsa_limit ) {
      continue;
    }

    op.clear_count();

    fsim.sppfp(tv, op);
    ++ pat_num;

    ymuint det_count = op.count(0);
    if ( det_count > 0 ) {
      tvlist.push_back(tv);
      tv = tvmgr.new_vector();
      ++ epat_num;
    }

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

  tvmgr.delete_vector(tv);

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(total_det_count, pat_num, epat_num, time);

  cout << "# of generated patterns = " << gnum << endl;
}

END_NAMESPACE_YM_SATPG_TD

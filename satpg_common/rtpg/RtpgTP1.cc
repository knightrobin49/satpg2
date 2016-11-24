
/// @file RtpgTP1.cc
/// @brief Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgTP1.h"
#include "FaultMgr.h"
#include "Tv2Mgr.h"
#include "TestVector2.h"
#include "FsimT.h"
#include "RtpgStats.h"
#include "FopRtpgT.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

RtpgTP*
new_RtpgTP1()
{
  RtpgTP1* rtpg = new RtpgTP1();
  return rtpg;
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgTP1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgTP1::RtpgTP1()
{
}

// @brief デストラクタ
RtpgTP1::~RtpgTP1()
{
}

// @brief 乱数生成器を初期化する．
// @param[in] seed 乱数の種
void
RtpgTP1::init(ymuint32 seed)
{
  mRandGen.init(seed);
}

// @brief RTPGを行なう．
// @param[in] fault_list 故障リスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[out] det_fault_list 検出された故障のリスト
// @param[out] tvlist テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
RtpgTP1::run(const vector<const TpgFault*>& fault_list,
	     Tv2Mgr& tvmgr,
	     FsimT& fsim,
	     ymuint min_f,
	     ymuint max_i,
	     ymuint max_pat,
	     ymuint wsa_limit,
	     vector<const TpgFault*>& det_fault_list,
	     vector<TestVector2*>& tvlist,
	     RtpgStats& stats)
{
  StopWatch local_timer;

  local_timer.start();

  ymuint fnum = fault_list.size();
  ymuint undet_i = 0;
  ymuint epat_num = 0;
  ymuint total_det_count = 0;

  TestVector2* tv = tvmgr.new_vector();

  FopRtpgT op(fsim);

  fsim.set_faults(fault_list);
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

  det_fault_list = op.fault_list();

  tvmgr.delete_vector(tv);

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(total_det_count, pat_num, epat_num, time);

  cout << "# of generated patterns = " << gnum << endl;
}

END_NAMESPACE_YM_SATPG

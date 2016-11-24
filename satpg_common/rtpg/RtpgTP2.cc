
/// @file RtpgTP2.cc
/// @brief Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgTP2.h"
#include "FaultMgr.h"
#include "Tv2Mgr.h"
#include "TestVector2.h"
#include "FsimT.h"
#include "RtpgStats.h"
#include "FopRtpgT.h"
#include "ym/StopWatch.h"
#include "ym/RandCombiGen.h"


BEGIN_NAMESPACE_YM_SATPG

RtpgTP*
new_RtpgTP2(ymuint nbits)
{
  RtpgTP2* rtpg = new RtpgTP2(nbits);
  return rtpg;
}

double
evaluate(ymuint sa,
	 ymuint thval)
{
  double thf = static_cast<double>(thval);
  double x = static_cast<double>(sa) - thf;
#if 0
  x /= thf;
  return 1.0 / (exp(x) + 1);
#else
#if 0
  return exp(- static_cast<double>(sa) / static_cast<double>(thval));
#else
  if ( sa < thval ) {
    return 1.0;
  }
  else {
    return 1.0 - (x / thval);
  }
#endif
#endif
}

void
gen_neighbor(const TestVector2* tv1,
	     TestVector2* tv2,
	     RandGen& randgen,
	     ymuint nbits)
{
  tv2->copy(*tv1);

  ymuint count = 0;
  for (ymuint i = 0; i < nbits; ++ i) {
    double r = randgen.real1();
    if ( r < 0.8 ) {
      ++ count;
    }
  }
  // count ビットだけ反転する．
  RandCombiGen rcg(tv1->input_num(), count);
  rcg.generate(randgen);
  for (ymuint i = 0; i < nbits; ++ i) {
    ymuint pos = rcg.elem(i);
    Val3 val = tv2->cur_val3(pos);
    tv2->set_cur_val(pos, ~val);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgTP2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] nbits 変更するビット数
RtpgTP2::RtpgTP2(ymuint nbits) :
  mNbits(nbits)
{
}

// @brief デストラクタ
RtpgTP2::~RtpgTP2()
{
}

// @brief 乱数生成器を初期化する．
// @param[in] seed 乱数の種
void
RtpgTP2::init(ymuint32 seed)
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
RtpgTP2::run(const vector<const TpgFault*>& fault_list,
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

  TestVector2* tv1 = tvmgr.new_vector();
  TestVector2* tv2 = tvmgr.new_vector();

  FopRtpgT op(fsim);

  fsim.set_faults(fault_list);
  op.init();

  ymuint gnum = 0;
  ymuint pat_num = 0;

  tv1->set_from_random(mRandGen);
  ymuint wsa0 = fsim.calc_wsa(tv1);
  double val0 = evaluate(wsa0, wsa_limit);
  ymuint count = 0;

  while ( pat_num < max_pat ) {
    // tv1 から tv2 を作る．
    gen_neighbor(tv1, tv2, mRandGen, mNbits);
    ++ gnum;
    ymuint wsa1 = fsim.calc_wsa(tv2);
    double val1 = evaluate(wsa1, wsa_limit);
    //cout << "wsa = " << wsa1 << ", val = " << val1
    //<< ", # of pat = " << pat_num << endl;

    bool accept = false;
    if ( val1 >= val0 ) {
      accept = true;
    }
    else {
      double r = mRandGen.real1();
      double ratio = val1 / val0;
      if ( r < ratio ) {
	accept = true;
      }
    }
    if ( accept ) {
      tv1->copy(*tv2);
      wsa0 = wsa1;
      val0 = val1;


      if ( wsa1 <= wsa_limit ) {
	op.clear_count();

	fsim.sppfp(tv1, op);
	++ pat_num;

	ymuint det_count = op.count(0);
	if ( det_count > 0 ) {
	  tvlist.push_back(tv1);
	  tv1 = tvmgr.new_vector();
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
    }
  }

  det_fault_list = op.fault_list();

  tvmgr.delete_vector(tv1);
  tvmgr.delete_vector(tv2);

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(total_det_count, pat_num, epat_num, time);

  cout << "# of generated patterns = " << gnum << endl;
}

END_NAMESPACE_YM_SATPG

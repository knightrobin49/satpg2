
/// @file Fsim2Test.cc
/// @brief Fsim2Test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "sa/TestVector.h"
#include "sa/TvMgr.h"
#include "sa/TvDeck.h"
#include "Fsim2.h"
#include "ym/RandGen.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

const char* argv0 = "";

// SPPFP のテスト
pair<ymuint, ymuint>
sppfp_test(Fsim2& fsim,
	   const vector<TestVector*>& tv_list)
{
  ymuint det_num = 0;
  ymuint nepat = 0;
  ymuint nv = tv_list.size();
  for (ymuint i = 0; i < nv; ++ i) {
    TestVector* tv = tv_list[i];
    vector<const TpgFault*> fault_list;
    fsim.sppfp(tv, fault_list);
    ymuint n = fault_list.size();
    if ( n > 0 ) {
      det_num += n;
      ++ nepat;
      for (ymuint j = 0; j < n; ++ j) {
	const TpgFault* f = fault_list[j];
	fsim.set_skip(f);
      }
    }
  }

  return make_pair(det_num, nepat);
}

// PPSFP のテスト
pair<ymuint, ymuint>
ppsfp_test(Fsim2& fsim,
	   const vector<const TestVector*>& tv_list)
{
  ymuint nv = tv_list.size();

  fsim.clear_patterns();
  ymuint wpos = 0;
  ymuint det_num = 0;
  ymuint nepat = 0;
  for (ymuint i = 0; i < nv; ++ i) {
    const TestVector* tv = tv_list[i];
    fsim.set_pattern(wpos, tv);
    ++ wpos;
    if ( wpos == kPvBitLen ) {
      vector<pair<const TpgFault*, PackedVal> > det_list;
      fsim.ppsfp(det_list);

      ymuint nb = wpos;
      PackedVal dpat_all = 0ULL;
      ymuint n = det_list.size();
      det_num += n;
      for (ymuint j = 0; j < n; ++ j) {
	const TpgFault* f = det_list[j].first;
	PackedVal dpat = det_list[j].second;
	fsim.set_skip(f);
	// dpat の最初の1のビットを求める．
	ymuint first = 0;
	for ( ; first < nb; ++ first) {
	  if ( dpat & (1ULL << first) ) {
	    break;
	  }
	}
	ASSERT_COND( first < nb );
	dpat_all |= (1ULL << first);
      }
      for (ymuint i = 0; i < nb; ++ i) {
	if ( dpat_all & (1ULL << i) ) {
	  ++ nepat;
	}
      }
      fsim.clear_patterns();
      wpos = 0;
    }
  }
  if ( wpos > 0 ) {
    vector<pair<const TpgFault*, PackedVal> > det_list;
    fsim.ppsfp(det_list);

    ymuint nb = wpos;
    PackedVal dpat_all = 0ULL;
    ymuint n = det_list.size();
    det_num += n;
    for (ymuint j = 0; j < n; ++ j) {
      const TpgFault* f = det_list[j].first;
      PackedVal dpat = det_list[j].second;
      fsim.set_skip(f);
      // dpat の最初の1のビットを求める．
      ymuint first = 0;
      for ( ; first < nb; ++ first) {
	if ( dpat & (1ULL << first) ) {
	  break;
	}
      }
      ASSERT_COND( first < nb );
      dpat_all |= (1ULL << first);
    }
    for (ymuint i = 0; i < nb; ++ i) {
      if ( dpat_all & (1ULL << i) ) {
	++ nepat;
      }
    }
  }

  return make_pair(det_num, nepat);
}

// ランダムにテストパタンを生成する．
// @param[in] rg 乱数発生器
// @param[in] tvmgr テストベクタを管理するオブジェクト
// @param[in] nv 生成するパタン数
// @param[out] tv_list 生成されたパタンを格納するベクタ
void
randgen(RandGen& rg,
	TvMgr& tvmgr,
	ymuint nv,
	vector<const TestVector*>& tv_list)
{
  tv_list.clear();
  tv_list.resize(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    TestVector* tv = tvmgr.new_vector();
    tv->set_from_random(rg);
    tv_list[i] = tv;
  }
}

void
usage()
{
  cerr << "USAGE: " << argv0 << " ?-n #pat? --blif|--iscas89 <file>" << endl;
}

int
fsim2test(int argc,
	  char** argv)
{
  ymuint npat = 0;
  bool blif = false;
  bool iscas89 = false;

  argv0 = argv[0];

  ymuint pos = 1;
  for ( ; pos < argc; ++ pos) {
    if ( argv[pos][0] == '-' ) {
      if ( strcmp(argv[pos], "-n") == 0 ) {
	++ pos;
	if ( pos >= argc ) {
	  cerr << " -n option requires #pat" << endl;
	  return -1;
	}
	npat = atoi(argv[pos]);
	if ( npat == 0 ) {
	  cerr << " integer expected after -n" << endl;
	  return -1;
	}
      }
      else if ( strcmp(argv[pos], "--blif") == 0 ) {
	if ( iscas89 ) {
	  cerr << "--blif and --iscas89 are mutual exclusive" << endl;
	  return -1;
	}
	blif = true;
      }
      else if ( strcmp(argv[pos], "--iscas89") == 0 ) {
	if ( blif ) {
	  cerr << "--blif and --iscas89 are mutual exclusive" << endl;
	  return -1;
	}
	iscas89 = true;
      }
    }
    else {
      break;
    }
  }

  if ( pos != argc - 1 ) {
    usage();
    return -1;
  }

  if ( npat == 0 ) {
    // デフォルトのパタン数を設定する．
    npat = 100000;
  }

  if ( !blif && !iscas89 ) {
    // とりあえず blif をデフォルトにする．
    blif = true;
  }

  string filename = argv[pos];
  TpgNetwork network;
  if ( blif ) {
    if ( !network.read_blif(filename) ) {
      cerr << "Error in reading " << filename << endl;
      return -1;
    }
  }
  else if ( iscas89 ) {
    if ( !network.read_iscas89(filename) ) {
      cerr << "Error in reading " << filename << endl;
      return -1;
    }
  }
  else {
    ASSERT_NOT_REACHED;
  }

  Fsim2 fsim;
  fsim.set_network(network);

  TvMgr tvmgr;
  tvmgr.init(network.ppi_num());

  RandGen rg;
  vector<const TestVector*> tv_list;

  randgen(rg, tvmgr, npat, tv_list);

  StopWatch timer;
  timer.start();

  pair<ymuint, ymuint> dpnum = ppsfp_test(fsim, tv_list);
  ymuint det_num = dpnum.first;
  ymuint nepat = dpnum.second;

  timer.stop();
  USTime time = timer.time();

  cout << "# of inputs             = " << network.ppi_num() << endl
       << "# of outputs            = " << network.ppo_num() << endl
       << "# of logic gates        = " << network.node_num() << endl
       << "# of MFFCs              = " << network.mffc_num() << endl
       << "# of FFRs               = " << network.ffr_num() << endl
       << "# of simulated patterns = " << npat << endl
       << "# of effective patterns = " << nepat << endl
       << "# of total faults       = " << network.rep_fault_num() << endl
       << "# of detected faults    = " << det_num << endl
       << "# of undetected faults  = " << network.rep_fault_num() - det_num << endl
       << "Total CPU time          = " << time << endl;

  return 0;
}

END_NAMESPACE_YM_SATPG_FSIM


int
main(int argc,
     char** argv)
{
  return nsYm::nsSatpg::nsSa::nsFsim2::fsim2test(argc, argv);
}

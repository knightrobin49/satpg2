
/// @file DtpgTest.cc
/// @brief DtpgTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "sa/DtpgS.h"
#include "sa/DtpgM.h"
#include "sa/BackTracer.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_SA

const char* argv0 = "";

pair<ymuint, ymuint>
single_test(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    const TpgNetwork& network,
	    FaultMgr& fmgr,
	    BackTracer& bt,
	    DtpgStats& stats)
{
  DtpgS dtpg_s(sat_type, sat_option, sat_outp, bt, network);

  ymuint detect_num = 0;
  ymuint untest_num = 0;
  ymuint nf = network.rep_fault_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = network.rep_fault(i);
    if ( fmgr.status(fault) == kFsUndetected ) {
      NodeValList nodeval_list;
      SatBool3 ans = dtpg_s.dtpg(fault, nodeval_list, stats);
      if ( ans == kB3True ) {
	++ detect_num;
      }
      else if ( ans == kB3False ) {
	++ untest_num;
      }
    }
  }

  return make_pair(detect_num, untest_num);
}

pair<ymuint, ymuint>
mffc_test(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp,
	  const TpgNetwork& network,
	  FaultMgr& fmgr,
	  BackTracer& bt,
	  DtpgStats& stats)
{
  ymuint detect_num = 0;
  ymuint untest_num = 0;

  ymuint nn = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);
    if ( node->imm_dom() != nullptr ) {
      continue;
    }

    DtpgM dtpg_m(sat_type, sat_option, sat_outp, bt, network, node);

    // node を根とする MFFC に含まれる故障のうち fault_list に入っていたものを求める．
    vector<const TpgFault*> f_list;
    ymuint nf = dtpg_m.fault_num();
    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* f = dtpg_m.fault(j);
      if ( fmgr.status(f) == kFsUndetected ) {
	f_list.push_back(f);
      }
    }
    if ( f_list.empty() ) {
      // 故障が残っていないのでパス
      continue;
    }

    dtpg_m.cnf_gen(stats);

    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* fault = f_list[j];
      if ( fmgr.status(fault) == kFsUndetected ) {
	// 故障に対するテスト生成を行なう．
	NodeValList nodeval_list;
	SatBool3 ans = dtpg_m.dtpg(fault, nodeval_list, stats);
	if ( ans == kB3True ) {
	  ++ detect_num;
	}
	else if ( ans == kB3False ) {
	  ++ untest_num;
	}
      }
    }
  }

  return make_pair(detect_num, untest_num);
}

void
usage()
{
  cerr << "USAGE: " << argv0 << " ?--single|--mffc? --blif|--iscas89 <file>" << endl;
}

int
dtpg_test(int argc,
	  char** argv)
{
  string sat_type;
  string sat_option;
  ostream* sat_outp = nullptr;

  bool blif = false;
  bool iscas89 = false;

  bool single = false;
  bool mffc = false;

  argv0 = argv[0];

  ymuint pos = 1;
  for ( ; pos < argc; ++ pos) {
    if ( argv[pos][0] == '-' ) {
      if ( strcmp(argv[pos], "--single") == 0 ) {
	if ( mffc ) {
	  cerr << "--single and --mffc are mutually exclusive" << endl;
	  return -1;
	}
	single = true;
      }
      else if ( strcmp(argv[pos], "--mffc") == 0 ) {
	if ( single ) {
	  cerr << "--single and --mffc are mutually exclusive" << endl;
	  return -1;
	}
	mffc = true;
      }
      else if ( strcmp(argv[pos], "--blif") == 0 ) {
	if ( iscas89 ) {
	  cerr << "--blif and --iscas89 are mutually exclusive" << endl;
	  return -1;
	}
	blif = true;
      }
      else if ( strcmp(argv[pos], "--iscas89") == 0 ) {
	if ( blif ) {
	  cerr << "--blif and --iscas89 are mutually exclusive" << endl;
	  return -1;
	}
	iscas89 = true;
      }
      else {
	cerr << argv[pos] << ": illegal option" << endl;
	usage();
	return -1;
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

  if ( !single && !mffc ) {
    // mffc をデフォルトにする．
    mffc = true;
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

  FaultMgr fmgr(network);

  ymuint xmode = 0;
  BackTracer bt(xmode, network.node_num());

  StopWatch timer;
  timer.start();

  DtpgStats stats;
  pair<ymuint, ymuint> num_pair;
#if 0
  if ( single ) {
    num_pair = single_test(sat_type, sat_option, sat_outp, network, fmgr, bt, stats);
  }
  else if ( mffc ) {
    num_pair = mffc_test(sat_type, sat_option, sat_outp, network, fmgr, bt, stats);
  }
  else {
    ASSERT_NOT_REACHED;
  }
#endif

  timer.stop();
  USTime time = timer.time();

  ymuint detect_num = num_pair.first;
  ymuint untest_num = num_pair.second;

  cout << "# of inputs             = " << network.ppi_num() << endl
       << "# of outputs            = " << network.ppo_num() << endl
       << "# of logic gates        = " << network.node_num() << endl
       << "# of MFFCs              = " << network.mffc_num() << endl
       << "# of FFRs               = " << network.ffr_num() << endl
       << "# of total faults       = " << network.rep_fault_num() << endl
       << "# of detected faults    = " << detect_num << endl
       << "# of untestable faults  = " << untest_num << endl
       << "Total CPU time          = " << time << endl;

  return 0;
}

END_NAMESPACE_YM_SATPG_SA


int
main(int argc,
     char** argv)
{
  return nsYm::nsSatpg::nsSa::dtpg_test(argc, argv);
}

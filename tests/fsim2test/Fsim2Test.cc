
/// @file Fsim2Test.cc
/// @brief Fsim2Test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "sa/TvMgr.h"
#include "sa/FsimOp.h"
#include "Fsim2.h"
#include "ym/RandGen.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

const char* argv0 = "";

//////////////////////////////////////////////////////////////////////
/// @class NopFop NopFop.h "NopFop.h"
/// @brief 何もしない FsimOp
//////////////////////////////////////////////////////////////////////
class NopFop :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  NopFop(Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~NopFop();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(const TpgFault* f,
	     PackedVal dpat);

  /// @brief 検出パタンをクリアする．
  void
  clear_pat();

  /// @brief 直前のシミュレーションで検出されたパタンのビットを返す．
  PackedVal
  det_pat();

  /// @brief 検出された故障数を返す．
  ymuint
  det_num();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 検出パタンのビット列
  PackedVal mDetPat;

  // 検出された故障数
  ymuint mDetNum;

};

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
NopFop::NopFop(Fsim& fsim) :
  mFsim(fsim)
{
  mDetNum = 0;
}

// @brief デストラクタ
NopFop::~NopFop()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
NopFop::operator()(const TpgFault* f,
		   PackedVal dpat)
{
  mFsim.set_skip(f);
  mDetPat |= dpat;
  ++ mDetNum;
}

// @brief 検出パタンをクリアする．
void
NopFop::clear_pat()
{
  mDetPat = kPvAll0;
}

// @brief 直前のシミュレーションで検出されたパタンのビットを返す．
PackedVal
NopFop::det_pat()
{
  return mDetPat;
}

// @brief 検出された故障数を返す．
ymuint
NopFop::det_num()
{
  return mDetNum;
}

// SPPFP のテスト
ymuint
sppfp_test(Fsim2& fsim,
	   NopFop& op,
	   const vector<TestVector*>& tv_list)
{
  ymuint nepat = 0;
  ymuint nv = tv_list.size();
  for (ymuint i = 0; i < nv; ++ i) {
    TestVector* tv = tv_list[i];
    op.clear_pat();
    fsim.sppfp(tv, op);
    if ( op.det_pat() != kPvAll0 ) {
      ++ nepat;
    }
  }

  return nepat;
}

// PPSFP のテスト
ymuint
ppsfp_test(Fsim2& fsim,
	   NopFop& op,
	   const vector<TestVector*>& tv_list)
{
  vector<TestVector*> tv_buff(kPvBitLen);
  ymuint nv = tv_list.size();
  ymuint nepat = 0;
  ymuint wpos = 0;
  for (ymuint i = 0; i < nv; ++ i) {
    TestVector* tv = tv_list[i];
    tv_buff[wpos] = tv;
    ++ wpos;
    if ( wpos == kPvBitLen ) {
      op.clear_pat();
      fsim.ppsfp(tv_buff, op);
      wpos = 0;
      PackedVal pat = op.det_pat();
      for (ymuint i = 0; i < kPvBitLen; ++ i) {
	if ( pat & (1ULL << i) ) {
	  ++ nepat;
	  break;
	}
      }
    }
  }
  if ( wpos > 0 ) {
    // 残りを tv_buff[0] と同じにする．
    TestVector* tv0 = tv_buff[0];
    for (ymuint j = wpos; j < kPvBitLen; ++ j) {
      tv_buff[j] = tv0;
    }
    op.clear_pat();
    fsim.ppsfp(tv_buff, op);
    PackedVal pat = op.det_pat();
    for (ymuint i = 0; i < wpos; ++ i) {
      if ( pat & (1ULL << i) ) {
	++ nepat;
	break;
      }
    }
  }

  return nepat;
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
	vector<TestVector*>& tv_list)
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
  vector<TestVector*> tv_list;

  randgen(rg, tvmgr, npat, tv_list);

  NopFop op(fsim);

  StopWatch timer;
  timer.start();

  ymuint nepat = ppsfp_test(fsim, op, tv_list);

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
       << "# of detected faults    = " << op.det_num() << endl
       << "# of undetected faults  = " << network.rep_fault_num() - op.det_num() << endl
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

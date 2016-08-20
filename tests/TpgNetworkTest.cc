
/// @file TpgNetworkTest.cc
/// @brief TpgNetworkTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

void
loop_test(ymuint loop_count,
	  const string& filename)
{
  TpgNetwork network;
  FaultMgr fmgr;
  for (ymuint i = 0; i < loop_count; ++ i) {
    if ( !network.read_iscas89(filename) ) {
      cerr << "Error in reading " << filename << endl;
      break;
    }
    fmgr.set_faults(network);
  }
}

END_NAMESPACE_YM_SATPG

int
main(int argc,
     char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " filename" << endl;
    return 255;
  }

  nsYm::nsSatpg::loop_test(1000, argv[1]);
}

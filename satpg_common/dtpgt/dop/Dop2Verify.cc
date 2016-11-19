
/// @file Dop2Verify.cc
/// @brief Dop2Verify の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2Verify.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'verify' タイプを生成する．
// @param[in] fsim 故障シミュレータ
DetectOp2*
new_Dop2Verify(Fsim& fsim)
{
  return new Dop2Verify(fsim);
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2Verify
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
Dop2Verify::Dop2Verify(Fsim& fsim) :
  mFsim(fsim)
{
}

// @brief デストラクタ
Dop2Verify::~Dop2Verify()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
Dop2Verify::operator()(const TpgFault* f,
		       const NodeVal2List& assign_list)
{
#if 0
  bool detect = mFsim.spsfp(assign_list, f);
  ASSERT_COND( detect );
#endif
}

END_NAMESPACE_YM_SATPG

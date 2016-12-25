
/// @file SpGate.cc
/// @brief SpGate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// @class SpGate SimPrim.h
// @brief 多入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpGate::SpGate(const vector<SimPrim*>& fanins) :
  mFaninNum(fanins.size()),
  mFanins(new SimPrim*[mFaninNum])
{
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    mFanins[i] = fanins[i];
  }
}

// @brief デストラクタ
SpGate::~SpGate()
{
  delete [] mFanins;
}


//////////////////////////////////////////////////////////////////////
// @class SpGate1 SimPrim.h
// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpGate1::SpGate1(const vector<SimPrim*>& fanins)
{
  ASSERT_COND(fanins.size() == 1 );
  mFanin = fanins[0];
}

// @brief デストラクタ
SpGate1::~SpGate1()
{
}


//////////////////////////////////////////////////////////////////////
// @class SpGate2 SimPrim.h
// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpGate2::SpGate2(const vector<SimPrim*>& fanins)
{
  ASSERT_COND(fanins.size() == 2 );
  mFanins[0] = fanins[0];
  mFanins[1] = fanins[1];
}

// @brief デストラクタ
SpGate2::~SpGate2()
{
}


//////////////////////////////////////////////////////////////////////
// @class SpGate3 SimPrim.h
// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpGate3::SpGate3(const vector<SimPrim*>& fanins)
{
  ASSERT_COND(fanins.size() == 3 );
  mFanins[0] = fanins[0];
  mFanins[1] = fanins[1];
  mFanins[2] = fanins[2];
}

// @brief デストラクタ
SpGate3::~SpGate3()
{
}


//////////////////////////////////////////////////////////////////////
// @class SpGate4 SimPrim.h
// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpGate4::SpGate4(const vector<SimPrim*>& fanins)
{
  ASSERT_COND(fanins.size() == 4 );
  mFanins[0] = fanins[0];
  mFanins[1] = fanins[1];
  mFanins[2] = fanins[2];
  mFanins[3] = fanins[3];
}

// @brief デストラクタ
SpGate4::~SpGate4()
{
}

END_NAMESPACE_YM_SATPG_FSIM

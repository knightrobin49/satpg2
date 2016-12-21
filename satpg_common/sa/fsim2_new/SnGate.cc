
/// @file SnGate.cc
/// @brief SnGate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// @class SnGate SimNode.h
// @brief 多入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate::SnGate(const vector<PackedVal*>& inputs) :
  mFaninNum(inputs.size()),
  mFaninVals(new PackedVal*[mFaninNum])
{
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    mFaninVals[i] = inputs[i];
  }
}

// @brief デストラクタ
SnGate::~SnGate()
{
  delete [] mFaninVals;
}


//////////////////////////////////////////////////////////////////////
// @class SnGate1 SimNode.h
// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate1::SnGate1(const vector<PackedVal*>& inputs)
{
  ASSERT_COND(inputs.size() == 1 );
  mFaninVal = inputs[0];
}

// @brief デストラクタ
SnGate1::~SnGate1()
{
}


//////////////////////////////////////////////////////////////////////
// @class SnGate2 SimNode.h
// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate2::SnGate2(const vector<PackedVal*>& inputs)
{
  ASSERT_COND(inputs.size() == 2 );
  mFaninVals[0] = inputs[0];
  mFaninVals[1] = inputs[1];
}

// @brief デストラクタ
SnGate2::~SnGate2()
{
}


//////////////////////////////////////////////////////////////////////
// @class SnGate3 SimNode.h
// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate3::SnGate3(const vector<PackedVal*>& inputs)
{
  ASSERT_COND(inputs.size() == 3 );
  mFaninVals[0] = inputs[0];
  mFaninVals[1] = inputs[1];
  mFaninVals[2] = inputs[2];
}

// @brief デストラクタ
SnGate3::~SnGate3()
{
}


//////////////////////////////////////////////////////////////////////
// @class SnGate4 SimNode.h
// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate4::SnGate4(const vector<PackedVal*>& inputs)
{
  ASSERT_COND(inputs.size() == 4 );
  mFaninVals[0] = inputs[0];
  mFaninVals[1] = inputs[1];
  mFaninVals[2] = inputs[2];
  mFaninVals[3] = inputs[3];
}

// @brief デストラクタ
SnGate4::~SnGate4()
{
}

END_NAMESPACE_YM_SATPG_FSIM

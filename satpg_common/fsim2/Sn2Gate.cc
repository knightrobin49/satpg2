
/// @file Sn2Gate.cc
/// @brief Sn2Gate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Gate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// クラス Sn2Gate
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Gate::Sn2Gate(ymuint id,
		 const vector<SimNode2*>& inputs) :
  SimNode2(id),
  mFaninNum(inputs.size()),
  mFanins(new SimNode2*[mFaninNum])
{
  ymuint max_level = 0;
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    SimNode2* input = inputs[i];
    ASSERT_COND(input );
    mFanins[i] = input;
    ymuint level = input->level() + 1;
    if ( max_level < level ) {
      max_level = level;
    }
  }
  set_level(max_level);
}

// @brief デストラクタ
Sn2Gate::~Sn2Gate()
{
  delete [] mFanins;
}

// @brief ファンイン数を得る．
ymuint
Sn2Gate::fanin_num() const
{
  return _fanin_num();
}

// @brief pos 番めのファンインを得る．
SimNode2*
Sn2Gate::fanin(ymuint pos) const
{
  ASSERT_COND( pos );
  return _fanin(pos);
}


//////////////////////////////////////////////////////////////////////
// @class Sn2Gate1 SimNode2.h
// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Gate1::Sn2Gate1(ymuint id,
		   const vector<SimNode2*>& inputs) :
  SimNode2(id)
{
  ASSERT_COND(inputs.size() == 1 );
  ASSERT_COND(inputs[0] );
  mFanin = inputs[0];
  set_level(mFanin->level() + 1);
}

// @brief デストラクタ
Sn2Gate1::~Sn2Gate1()
{
}

// @brief ファンイン数を得る．
ymuint
Sn2Gate1::fanin_num() const
{
  return 1;
}

// @brief pos 番めのファンインを得る．
SimNode2*
Sn2Gate1::fanin(ymuint pos) const
{
  return mFanin;
}


//////////////////////////////////////////////////////////////////////
// @class Sn2Gate2 SimNode2.h
// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Gate2::Sn2Gate2(ymuint id,
		   const vector<SimNode2*>& inputs) :
  SimNode2(id)
{
  ASSERT_COND(inputs.size() == 2 );
  ASSERT_COND(inputs[0] );
  ASSERT_COND(inputs[1] );
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  ymuint level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
Sn2Gate2::~Sn2Gate2()
{
}

// @brief ファンイン数を得る．
ymuint
Sn2Gate2::fanin_num() const
{
  return 2;
}

// @brief pos 番めのファンインを得る．
SimNode2*
Sn2Gate2::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 2 );
  return _fanin(pos);
}


//////////////////////////////////////////////////////////////////////
// @class Sn2Gate3 SimNode2.h
// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Gate3::Sn2Gate3(ymuint id,
		   const vector<SimNode2*>& inputs) :
  SimNode2(id)
{
  ASSERT_COND(inputs.size() == 3 );
  ASSERT_COND(inputs[0] );
  ASSERT_COND(inputs[1] );
  ASSERT_COND(inputs[2] );
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  mFanins[2] = inputs[2];
  ymuint level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  if ( level < mFanins[2]->level() ) {
    level = mFanins[2]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
Sn2Gate3::~Sn2Gate3()
{
}

// @brief ファンイン数を得る．
ymuint
Sn2Gate3::fanin_num() const
{
  return 3;
}

// @brief pos 番めのファンインを得る．
SimNode2*
Sn2Gate3::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 3 );
  return _fanin(pos);
}


//////////////////////////////////////////////////////////////////////
// @class Sn2Gate4 SimNode2.h
// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Gate4::Sn2Gate4(ymuint id,
		   const vector<SimNode2*>& inputs) :
  SimNode2(id)
{
  ASSERT_COND(inputs.size() == 4 );
  ASSERT_COND(inputs[0] );
  ASSERT_COND(inputs[1] );
  ASSERT_COND(inputs[2] );
  ASSERT_COND(inputs[3] );
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  mFanins[2] = inputs[2];
  mFanins[3] = inputs[3];
  ymuint level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  if ( level < mFanins[2]->level() ) {
    level = mFanins[2]->level();
  }
  if ( level < mFanins[3]->level() ) {
    level = mFanins[3]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
Sn2Gate4::~Sn2Gate4()
{
}

// @brief ファンイン数を得る．
ymuint
Sn2Gate4::fanin_num() const
{
  return 4;
}

// @brief pos 番めのファンインを得る．
SimNode2*
Sn2Gate4::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 4 );
  return _fanin(pos);
}

END_NAMESPACE_YM_SATPG_FSIM

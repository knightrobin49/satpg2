
/// @file TvMgr.cc
/// @brief TvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "sa/TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG_SA

// @brief コンストラクタ
TvMgr::TvMgr() :
  mAlloc(nullptr),
  mInputNum(0)
{
}

// @brief デストラクタ
TvMgr::~TvMgr()
{
  clear();
}

/// @brief 内容を空にする．
void
TvMgr::clear()
{
  if ( mAlloc != nullptr ) {
    delete mAlloc;
    mAlloc = nullptr;
    mInputNum = 0;
  }
}

// @brief 初期化する．
// @param[in] ni 入力数
//
// 副作用で clear() が呼ばれる．
void
TvMgr::init(ymuint ni)
{
  clear();

  mInputNum = ni;
  if ( mInputNum == 0 ) {
    // 0 だとヤバい
    mInputNum = 1;
  }

  ymuint nb = TestVector::block_num(mInputNum);
  mTvSize = sizeof(TestVector) + kPvBitLen * (nb - 1);
  mAlloc = new UnitAlloc(mTvSize, 1024);
}

// @brief 新しいパタンを生成する．
// @return 生成されたパタンを返す．
// @note パタンは0で初期化される．
TestVector*
TvMgr::new_vector()
{
  void* p = mAlloc->get_memory(mTvSize);
  TestVector* tv = new (p) TestVector(mInputNum);

  return tv;
}

// @brief パタンを削除する．
void
TvMgr::delete_vector(TestVector* tv)
{
  mAlloc->put_memory(mTvSize, (void*)tv);
}

// @brief 入力数を返す．
ymuint
TvMgr::input_num() const
{
  return mInputNum;
}

END_NAMESPACE_YM_SATPG_SA


/// @file Tv2Mgr.cc
/// @brief Tv2Mgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Tv2Mgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
Tv2Mgr::Tv2Mgr() :
  mAlloc(nullptr),
  mVectLen(0),
  mInputNum(0),
  mTvSize(0)
{
}

// @brief デストラクタ
Tv2Mgr::~Tv2Mgr()
{
  clear();
}

/// @brief 内容を空にする．
void
Tv2Mgr::clear()
{
  if ( mAlloc != nullptr ) {
    delete mAlloc;
    mAlloc = nullptr;
    mVectLen = 0;
    mInputNum = 0;
    mTvSize = 0;
  }
}

// @brief 初期化する．
// @param[in] input_num 入力数
//
// 副作用で clear() が呼ばれる．
void
Tv2Mgr::init(ymuint input_num)
{
  clear();

  mInputNum = input_num;
  mVectLen = input_num + input_num;
  if ( mVectLen == 0 ) {
    // 0 だとヤバい
    mVectLen = 1;
  }

  ymuint nb = TestVector2::block_num(mVectLen);
  mTvSize = sizeof(TestVector2) + kPvBitLen * (nb - 1);
  mAlloc = new UnitAlloc(mTvSize, 1024);
}

// @brief 新しいパタンを生成する．
// @return 生成されたパタンを返す．
// @note パタンは0で初期化される．
TestVector2*
Tv2Mgr::new_vector()
{
  void* p = mAlloc->get_memory(mTvSize);
  TestVector2* tv = new (p) TestVector2(mInputNum);

  // X に初期化しておく．
  ymuint nb = TestVector2::block_num(mVectLen);
  for (ymuint i = 0; i < nb; ++ i) {
    tv->mPat[i] = kPvAll0;
  }

  return tv;
}

// @brief パタンを削除する．
void
Tv2Mgr::delete_vector(TestVector2* tv)
{
  mAlloc->put_memory(mTvSize, (void*)tv);
}

// @brief ベクタ長を返す．
ymuint
Tv2Mgr::vect_len() const
{
  return mVectLen;
}

// @brief 入力数を返す．
ymuint
Tv2Mgr::input_num() const
{
  return mInputNum;
}

END_NAMESPACE_YM_SATPG

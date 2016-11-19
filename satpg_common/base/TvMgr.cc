
/// @file TvMgr.cc
/// @brief TvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
TvMgr::TvMgr() :
  mAlloc(nullptr),
  mVectLen(0),
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
    mVectLen = 0;
    mInputNum = 0;
  }
}

// @brief 初期化する．
// @param[in] vect_len ベクタ長
// @param[in] ni 入力数
//
// 副作用で clear() が呼ばれる．
// 組み合わせ回路(スキャン方式)の時は vect_len == ni だが
// ブロードサイド方式の時は vect_len != ni となる．
void
TvMgr::init(ymuint vect_len,
	    ymuint ni)
{
  clear();

  mVectLen = vect_len;
  mInputNum = ni;
  if ( mVectLen == 0 ) {
    // 0 だとヤバい
    mVectLen = 1;
  }

  ymuint nb = TestVector::block_num(mVectLen);
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
  TestVector* tv = new (p) TestVector(mVectLen);

  // X に初期化しておく．
  ymuint nb = TestVector::block_num(mVectLen);
  for (ymuint i = 0; i < nb; ++ i) {
    tv->mPat[i] = kPvAll0;
  }

  return tv;
}

// @brief パタンを削除する．
void
TvMgr::delete_vector(TestVector* tv)
{
  mAlloc->put_memory(mTvSize, (void*)tv);
}

// @brief ベクタ長を返す．
ymuint
TvMgr::vect_len() const
{
  return mVectLen;
}

// @brief 入力数を返す．
ymuint
TvMgr::input_num() const
{
  return mInputNum;
}

END_NAMESPACE_YM_SATPG

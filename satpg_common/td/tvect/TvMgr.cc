
/// @file TvMgr.cc
/// @brief TvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "td/TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG_TD

// @brief コンストラクタ
TvMgr::TvMgr() :
  mAlloc(nullptr),
  mVectLen(0),
  mInputNum(0),
  mDffNum(0),
  mTvSize(0)
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
    mDffNum = 0;
    mTvSize = 0;
  }
}

// @brief 初期化する．
// @param[in] input_num 入力数
// @param[in] dff_num DFF数
//
// 副作用で最初に clear() が呼ばれる．
void
TvMgr::init(ymuint input_num,
	    ymuint dff_num)
{
  clear();

  mInputNum = input_num;
  mDffNum = dff_num;
  mVectLen = input_num + input_num + dff_num;
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
  TestVector* tv = new (p) TestVector(mInputNum, mDffNum);

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

// @brief DFF数を返す．
ymuint
TvMgr::dff_num() const
{
  return mDffNum;
}

END_NAMESPACE_YM_SATPG_TD

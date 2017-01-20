#ifndef TVDECK_H
#define TVDECK_H

/// @file TvDeck.h
/// @brief TvDeck のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

#include "sa/sa_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class TvDeck TvDeck.h "TvDeck.h"
/// @brief ppsfp 用に TestVector をまとめて保持するためのクラス
///
/// 中身はただの TestVector* の可変配列だが要素数が最大で kPvBitLen
/// という制約を持つ．<br>
/// Fsim::ppsfp() の入力として使うことを想定している．
//////////////////////////////////////////////////////////////////////
class TvDeck
{
public:

  /// @brief コンストラクタ
  TvDeck();

  /// @brief デストラクタ
  ~TvDeck();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 空の時 true を返す．
  bool
  is_empty() const;

  /// @brief 満杯の時 true を返す．
  bool
  is_full() const;

  /// @brief 要素を取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < num() )
  const TestVector*
  operator[](ymuint pos) const;

  /// @brief 要素を追加する．
  /// @param[in] tv 追加する要素
  ///
  /// is_full() == false でなければならない．
  void
  add(const TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mNum;

  // 要素の配列
  const TestVector* mBody[kPvBitLen];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TvDeck::TvDeck()
{
  mNum = 0;
}

// @brief デストラクタ
inline
TvDeck::~TvDeck()
{
}

// @brief 内容をクリアする．
inline
void
TvDeck::clear()
{
  mNum = 0;
}

// @brief 要素数を返す．
inline
ymuint
TvDeck::num() const
{
  return mNum;
}

// @brief 空の時 true を返す．
inline
bool
TvDeck::is_empty() const
{
  return num() == 0;
}

// @brief 満杯の時 true を返す．
inline
bool
TvDeck::is_full() const
{
  return num() == kPvBitLen;
}

// @brief 要素を取り出す．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
inline
const TestVector*
TvDeck::operator[](ymuint pos) const
{
  ASSERT_COND( pos < num() );
  return mBody[pos];
}

// @brief 要素を追加する．
// @param[in] tv 追加する要素
//
// is_full() == false でなければならない．
inline
void
TvDeck::add(const TestVector* tv)
{
  ASSERT_COND( num() < kPvBitLen );
  mBody[mNum] = tv;
  ++ mNum;
}

END_NAMESPACE_YM_SATPG_SA

#endif // TVDECK_H

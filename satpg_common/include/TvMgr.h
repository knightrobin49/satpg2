#ifndef TVMGR_H
#define TVMGR_H

/// @file TvMgr.h
/// @brief TvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2013-2014 Yusuke Matsunaga
/// All rights reserved.

#include "satpg.h"
#include "TestVector.h"
#include "ym/UnitAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TvMgr TvMgr.h "TvMgr.h"
/// @brief テストベクタを管理するクラス
//////////////////////////////////////////////////////////////////////
class TvMgr
{
public:

  /// @brief コンストラクタ
  TvMgr();

  /// @brief デストラクタ
  /// @note このオブジェクトが確保したすべてのテストベクタを開放する．
  ~TvMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  /// @note このオブジェクトが生成した全てのテストベクタが開放される．
  void
  clear();

  /// @brief 初期化する．
  /// @param[in] vect_len ベクタ長
  /// @param[in] ni 入力数
  ///
  /// 副作用で clear() が呼ばれる．
  /// 組み合わせ回路(スキャン方式)の時は vect_len == ni だが
  /// ブロードサイド方式の時は vect_len != ni となる．
  void
  init(ymuint vect_len,
       ymuint ni);

  /// @brief 新しいパタンを生成する．
  /// @return 生成されたパタンを返す．
  /// @note パタンは0で初期化される．
  TestVector*
  new_vector();

  /// @brief パタンを削除する．
  void
  delete_vector(TestVector* tv);

  /// @brief ベクタ長を返す．
  ymuint
  vect_len() const;

  /// @brief 入力数を返す．
  ymuint
  input_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 使用禁止にするためのプライベート宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーコンストラクタ
  /// @note 使用禁止なので実装しない．
  TvMgr(const TvMgr& src);

  /// @brief 代入演算子
  /// @note 使用禁止なので実装しない．
  const TvMgr&
  operator=(const TvMgr& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタのメモリ確保用のアロケータ
  UnitAlloc* mAlloc;

  // テストベクタの長さ
  ymuint mVectLen;

  // 対象回路の入力数
  ymuint mInputNum;

  // TestVector の実際のサイズ
  ymuint mTvSize;

};

END_NAMESPACE_YM_SATPG

#endif // TVMGR_H

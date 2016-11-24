#ifndef TV2MGR_H
#define TV2MGR_H

/// @file Tv2Mgr.h
/// @brief Tv2Mgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2013-2014 Yusuke Matsunaga
/// All rights reserved.

#include "satpg.h"
#include "TestVector2.h"
#include "ym/UnitAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Tv2Mgr Tv2Mgr.h "Tv2Mgr.h"
/// @brief テストベクタを管理するクラス
//////////////////////////////////////////////////////////////////////
class Tv2Mgr
{
public:

  /// @brief コンストラクタ
  Tv2Mgr();

  /// @brief デストラクタ
  /// @note このオブジェクトが確保したすべてのテストベクタを開放する．
  ~Tv2Mgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  /// @note このオブジェクトが生成した全てのテストベクタが開放される．
  void
  clear();

  /// @brief 初期化する．
  /// @param[in] input_num 入力数
  ///
  /// 副作用で clear() が呼ばれる．
  void
  init(ymuint input_num);

  /// @brief 新しいパタンを生成する．
  /// @return 生成されたパタンを返す．
  /// @note パタンは0で初期化される．
  TestVector2*
  new_vector();

  /// @brief パタンを削除する．
  void
  delete_vector(TestVector2* tv);

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
  Tv2Mgr(const Tv2Mgr& src);

  /// @brief 代入演算子
  /// @note 使用禁止なので実装しない．
  const Tv2Mgr&
  operator=(const Tv2Mgr& src);


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

  // TestVector2 の実際のサイズ
  ymuint mTvSize;

};

END_NAMESPACE_YM_SATPG

#endif // TV2MGR_H

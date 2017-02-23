#ifndef TPGFFR_H
#define TPGFFR_H

/// @file TpgFFR.h
/// @brief TpgFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgFFR TpgFFR.h "TpgFFR.h"
/// @brief TpgNetwork の FFR の情報を表すクラス
/// @sa TpgNetwork
/// @sa TpgNode
/// @sa TpgFault
/// @sa TpgMFFC
///
/// 具体的には以下の情報を持つ．
/// - FFR の根のノード
/// - FFR の要素のノードのリスト
/// - FFR に含まれる代表故障のリスト
/// 一度設定された不変のオブジェクトとなる．
//////////////////////////////////////////////////////////////////////
class TpgFFR
{
public:

  /// @brief コンストラクタ
  TpgFFR();

  /// @brief デストラクタ
  ~TpgFFR();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを返す．
  const TpgNode*
  root() const;

  /// @brief このFFRに含まれるノード数を返す．
  ymuint
  elem_num() const;

  /// @brief このFFRに含まれるノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  const TpgNode*
  elem(ymuint pos) const;

  /// @brief このFFRに含まれる代表故障の数を返す．
  ymuint
  fault_num() const;

  /// @brief このFFRに含まれる代表故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  fault(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] node_list ノードのリスト
  /// @param[in] fault_list 故障のリスト
  /// @param[in] alloc メモリアロケータ
  void
  set(vector<TpgNode*>& node_list,
      vector<TpgFault*>& fault_list,
      Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint mElemNum;

  // ノードの配列
  const TpgNode** mElemList;

  // 故障数
  ymuint mFaultNum;

  // 故障の配列
  const TpgFault** mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TpgFFR::TpgFFR()
{
  mElemNum = 0;
  mElemList = nullptr;
  mFaultNum = 0;
  mFaultList = nullptr;
}

// @brief デストラクタ
inline
TpgFFR::~TpgFFR()
{
}

// @brief 根のノードを返す．
inline
const TpgNode*
TpgFFR::root() const
{
  return mElemList[0];
}

// @brief このFFRに含まれるノード数を返す．
inline
ymuint
TpgFFR::elem_num() const
{
  return mElemNum;
}

// @brief このFFRに含まれるノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
inline
const TpgNode*
TpgFFR::elem(ymuint pos) const
{
  ASSERT_COND( pos < elem_num() );

  return mElemList[pos];
}

// @brief このFFRに含まれる代表故障の数を返す．
inline
ymuint
TpgFFR::fault_num() const
{
  return mFaultNum;
}

// @brief このFFRに含まれる代表故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
inline
const TpgFault*
TpgFFR::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );

  return mFaultList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // TPGFFR_H

#ifndef CPLXTPGNODEINFO_H
#define CPLXTPGNODEINFO_H

/// @file CplxTpgNodeInfo.h
/// @brief CplxTpgNodeInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

#include "TpgNodeInfo.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class CplxTpgNodeInfo CplxTpgNodeInfo.h "CplxTpgNodeInfo.h"
/// @brief 複合型の TpgNodeInfo
//////////////////////////////////////////////////////////////////////
class CplxTpgNodeInfo :
  public TpgNodeInfo
{
public:

  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] expr 論理式
  CplxTpgNodeInfo(ymuint ni,
		  const Expr& expr);

  /// @brief デストラクタ
  virtual
  ~CplxTpgNodeInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;

  /// @brief 論理式を返す．
  virtual
  Expr
  expr() const;

  /// @brief 追加ノード数を返す．
  virtual
  ymuint
  extra_node_num() const;

  /// @brief 制御値を返す．
  /// @param[in] pos 入力位置
  /// @param[in] val 値
  virtual
  Val3
  cval(ymuint pos,
       Val3 val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理式
  Expr mExpr;

  // 追加のノード数
  ymuint mExtraNodeNum;

  // 制御値の配列
  vector<Val3> mCVal;

};

END_NAMESPACE_YM_SATPG

#endif // CPLXTPGNODEINFO_H

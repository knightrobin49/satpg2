#ifndef SIMPLETPGNODEINFO_H
#define SIMPLETPGNODEINFO_H

/// @file SimpleTpgNodeInfo.h
/// @brief SimpleTpgNodeInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNodeInfo.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SimpleTpgNodeInfo SimpleTpgNodeInfo.h "SimpleTpgNodeInfo.h"
/// @brief 組み込み型の TpgNodeInfo
//////////////////////////////////////////////////////////////////////
class SimpleTpgNodeInfo :
  public TpgNodeInfo
{
public:

  /// @brief コンストラクタ
  /// @param[in] gate_type ゲートタイプ
  SimpleTpgNodeInfo(GateType gate_type);

  /// @brief デストラクタ
  virtual
  ~SimpleTpgNodeInfo();


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

  // ゲートタイプ
  GateType mGateType;

  // 制御値
  Val3 mCVal[2];

};

END_NAMESPACE_YM_SATPG

#endif // SIMPLETPGNODEINFO_H

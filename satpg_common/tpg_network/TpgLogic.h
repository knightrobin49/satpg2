#ifndef TPGLOGIC_H
#define TPGLOGIC_H

/// @file TpgLogic.h
/// @brief TpgLogic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogic TpgLogic.h "TpgLogic.h"
/// @brief 論理ノードの基底クラス
//////////////////////////////////////////////////////////////////////
class TpgLogic :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  TpgLogic(ymuint id,
	   const char* name);

  /// @brief デストラクタ
  ~TpgLogic();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief logic タイプの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  const TpgFault*
  output_fault(int val) const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  TpgFault*
  output_fault(int val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] fault 故障
  virtual
  void
  set_output_fault(int val,
		   TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の故障
  TpgFault* mOutputFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGLOGIC_H

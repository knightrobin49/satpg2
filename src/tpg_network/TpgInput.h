#ifndef TPGINPUT_H
#define TPGINPUT_H

/// @file TpgInput.h
/// @brief TpgInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgInput TpgInput.h "TpgInput.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgInput :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] input_id 入力番号
  TpgInput(ymuint id,
	   ymuint input_id);

  /// @brief デストラクタ
  virtual
  ~TpgInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  virtual
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ///
  /// node = TpgNetwork::input(node->input_id()
  /// の関係を満たす．
  /// is_input() が false の場合の返り値は不定
  virtual
  ymuint
  input_id() const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  const TpgFault*
  output_fault(int val) const;

  /// @brief このノードに関係する故障数を返す．
  virtual
  ymuint
  fault_num() const;

  /// @brief このノードに関係する故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  virtual
  const TpgFault*
  fault(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号
  ymuint32 mInputId;

  // 故障
  TpgFault* mFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGINPUT_H

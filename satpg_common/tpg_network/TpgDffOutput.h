#ifndef TPGDFFOUTPUT_H
#define TPGDFFOUTPUT_H

/// @file TpgDffOutput.h
/// @brief TpgDffOutput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgInput.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgDffOutput TpgDffOutput.h "TpgDffOutput.h"
/// @brief DFF の出力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgDffOutput :
  public TpgInput
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] input_id 入力番号
  /// @param[in] fanout_num ファンアウト数
  TpgDffOutput(ymuint id,
	       ymuint input_id,
	       ymuint fanout_num);

  /// @brief デストラクタ
  virtual
  ~TpgDffOutput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFF の出力に接続している外部入力タイプの時 true を返す．
  virtual
  bool
  is_dff_output() const;

  /// @brief DFF の出力に接続している外部入力タイプの時に対応する外部出力を返す．
  ///
  /// is_dff_output() == false の時には nullptr を返す．
  virtual
  TpgNode*
  alt_output() const;

  /// @brief DFFの入出力の時に相方のノードを設定する．
  virtual
  void
  set_alt_node(TpgNode* alt_node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する DFF の入力ノード
  TpgNode* mAltNode;

};

END_NAMESPACE_YM_SATPG

#endif // TPGDFFOUTPUT_H

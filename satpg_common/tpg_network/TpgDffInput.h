#ifndef TPGDFFINPUT_H
#define TPGDFFINPUT_H

/// @file TpgDffInput.h
/// @brief TpgDffInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOutput.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgDffInput TpgDffInput.h "TpgDffInput.h"
/// @brief DFFの入力を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgDffInput :
  public TpgOutput
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] output_id 出力番号
  /// @param[in] fanin ファンインのノード
  TpgDffInput(ymuint id,
	      ymuint output_id,
	      TpgNode* fanin);

  /// @brief デストラクタ
  ~TpgDffInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFF の入力に接続している外部出力タイプの時 true を返す．
  virtual
  bool
  is_dff_input() const;

  /// @brief DFF の入力に接続している外部出力タイプの時に対応する外部入力を返す．
  ///
  /// is_dff_input() == false の時には nullptr を返す．
  virtual
  TpgNode*
  alt_input() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFの入出力の時に相方のノードを設定する．
  virtual
  void
  set_alt_node(TpgNode* alt_node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する DFF の出力ノード
  TpgNode* mAltNode;

};

END_NAMESPACE_YM_SATPG

#endif // TPGDFFINPUT_H

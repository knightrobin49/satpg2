#ifndef TPGNODEIMPL_H
#define TPGNODEIMPL_H

/// @file TpgNodeImpl.h
/// @brief TpgNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgNodeImpl TpgNodeImpl.h "TpgNodeImpl.h"
/// @brief TpgNode の実装クラス
//////////////////////////////////////////////////////////////////////
class TpgNodeImpl :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  TpgNodeImpl();

  /// @brief デストラクタ
  ~TpgNodeImpl();


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

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  virtual
  bool
  is_output() const;

  /// @brief 外部出力タイプの時に出力番号を返す．
  ///
  /// node = TpgNetwork::output(node->output_id())
  /// の関係を満たす．
  /// is_output() が false の場合の返り値は不定
  virtual
  ymuint
  output_id() const;

  /// @brief TFIサイズの昇順に並べた時の出力番号を返す．
  virtual
  ymuint
  output_id2() const;

  /// @brief logic タイプの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief 根のノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  virtual
  bool
  is_root() const;

  /// @brief もとのゲートのファンインに対応するノードを返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  virtual
  TpgNode*
  input_map(ymuint pos) const;

  /// @brief BnNode のファンインに対応するノードのファンイン番号を返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  virtual
  ymuint
  ipos_map(ymuint pos) const;

  /// @brief 内部ノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  virtual
  bool
  is_internal() const;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_SATPG

#endif // TPGNODEIMPL_H

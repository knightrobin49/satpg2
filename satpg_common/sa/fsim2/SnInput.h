#ifndef SNINPUT_H
#define SNINPUT_H

/// @file SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnInput SimNode.h
/// @brief 入力ノード
//////////////////////////////////////////////////////////////////////
class SnInput :
  public SimNode
{
public:

  /// @brief コンストラクタ
  explicit
  SnInput(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SnInput();


public:

  /// @brief ゲートタイプを返す．
  ///
  /// ここでは kGateBUFF を返す．
  virtual
  GateType
  gate_type() const;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(ymuint pos) const;

  /// @brief 正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  /// @retval true 新しい値に更新された．
  /// @retval false 値は変わらなかった．
  /// @note 結果は mFval にセットされる．
  virtual
  PackedVal
  _calc_fval();

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  _calc_lobs(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNINPUT_H

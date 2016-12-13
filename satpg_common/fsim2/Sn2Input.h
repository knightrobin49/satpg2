#ifndef SN2INPUT_H
#define SN2INPUT_H

/// @file Sn2Input.h
/// @brief Sn2Input のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode.h"
#include "Sn2Gate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class Sn2Input SimNode.h
/// @brief 入力ノード
//////////////////////////////////////////////////////////////////////
class Sn2Input :
  public SimNode2
{
public:

  /// @brief コンストラクタ
  explicit
  Sn2Input(ymuint id);

  /// @brief デストラクタ
  virtual
  ~Sn2Input();


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


//////////////////////////////////////////////////////////////////////
/// @class Sn2Buff SimNode.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class Sn2Buff :
  public Sn2Gate1
{
public:

  /// @brief コンストラクタ
  Sn2Buff(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Buff();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;

  /// @brief 正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
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


//////////////////////////////////////////////////////////////////////
/// @class Sn2Not SimNode.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class Sn2Not :
  public Sn2Buff
{
public:

  /// @brief コンストラクタ
  Sn2Not(ymuint32 id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Not();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;

  /// @brief 正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  PackedVal
  _calc_fval();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SN2INPUT_H

#ifndef SNINPUT_H
#define SNINPUT_H

/// @file SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode.h"
#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_TFSIM2

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
  SnInput(ymuint32 id);

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
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(ymuint pos) const;

  /// @brief 1時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval1();

  /// @brief 2時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．
  virtual
  PackedVal
  _calc_fval2();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnBuff SimNode.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class SnBuff :
  public SnGate1
{
public:

  /// @brief コンストラクタ
  SnBuff(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnBuff();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;

  /// @brief 1時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval1();

  /// @brief 2時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．
  virtual
  PackedVal
  _calc_fval2();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNot SimNode.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class SnNot :
  public SnBuff
{
public:

  /// @brief コンストラクタ
  SnNot(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNot();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;

  /// @brief 1時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval1();

  /// @brief 2時刻目の正常値の計算を行う．
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．
  virtual
  PackedVal
  _calc_fval2();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SATPG_TFSIM2

#endif // SNINPUT_H

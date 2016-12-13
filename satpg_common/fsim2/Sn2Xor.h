#ifndef SN2XOR_H
#define SN2XOR_H

/// @file Sn2Xor.h
/// @brief Sn2Xor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Gate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class Sn2Xor Sn2Xor.h
/// @brief XORノード
//////////////////////////////////////////////////////////////////////
class Sn2Xor :
  public Sn2Gate
{
public:

  /// @brief コンストラクタ
  Sn2Xor(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Xor();


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
/// @class Sn2Nor2 Sn2Xor.h
/// @brief 2入力XORノード
//////////////////////////////////////////////////////////////////////
class Sn2Xor2 :
  public Sn2Gate2
{
public:

  /// @brief コンストラクタ
  Sn2Xor2(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Xor2();


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
/// @class Sn2Xnor Sn2Xor.h
/// @brief XNORノード
//////////////////////////////////////////////////////////////////////
class Sn2Xnor :
  public Sn2Xor
{
public:

  /// @brief コンストラクタ
  Sn2Xnor(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Xnor();


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


//////////////////////////////////////////////////////////////////////
/// @class Sn2Nor2 Sn2Xor.h
/// @brief 2入力XNORノード
//////////////////////////////////////////////////////////////////////
class Sn2Xnor2 :
  public Sn2Xor2
{
public:

  /// @brief コンストラクタ
  Sn2Xnor2(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Xnor2();


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

#endif // SN2XOR_H

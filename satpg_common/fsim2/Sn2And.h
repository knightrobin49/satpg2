#ifndef SN2AND_H
#define SN2AND_H

/// @file Sn2And.h
/// @brief Sn2And のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class Sn2And Sn2And.h
/// @brief ANDノード
//////////////////////////////////////////////////////////////////////
class Sn2And :
  public SnGate
{
public:

  /// @brief コンストラクタ
  Sn2And(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2And();


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
/// @class Sn2And2 Sn2And.h
/// @brief 2入力ANDノード
//////////////////////////////////////////////////////////////////////
class Sn2And2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  Sn2And2(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2And2();


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
/// @class Sn2And3 Sn2And.h
/// @brief 3入力ANDノード
//////////////////////////////////////////////////////////////////////
class Sn2And3 :
  public SnGate3
{
public:

  /// @brief コンストラクタ
  Sn2And3(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2And3();


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
/// @class Sn2And4 Sn2And.h
/// @brief 4入力ANDノード
//////////////////////////////////////////////////////////////////////
class Sn2And4 :
  public SnGate4
{
public:

  /// @brief コンストラクタ
  Sn2And4(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2And4();


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
  _calc_gobs(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class Sn2Nand Sn2And.h
/// @brief NANDノード
//////////////////////////////////////////////////////////////////////
class Sn2Nand :
  public Sn2And
{
public:

  /// @brief コンストラクタ
  Sn2Nand(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nand();


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
/// @class Sn2Nand2 Sn2And.h
/// @brief 2入力NANDノード
//////////////////////////////////////////////////////////////////////
class Sn2Nand2 :
  public Sn2And2
{
public:

  /// @brief コンストラクタ
  Sn2Nand2(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nand2();


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
/// @class Sn2Nand3 Sn2And.h
/// @brief 3入力NANDノード
//////////////////////////////////////////////////////////////////////
class Sn2Nand3 :
  public Sn2And3
{
public:

  /// @brief コンストラクタ
  Sn2Nand3(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nand3();


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
/// @class Sn2Nand4 Sn2And.h
/// @brief 4入力NANDノード
//////////////////////////////////////////////////////////////////////
class Sn2Nand4 :
  public Sn2And4
{
public:

  /// @brief コンストラクタ
  Sn2Nand4(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nand4();


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

#endif // SN2AND_H

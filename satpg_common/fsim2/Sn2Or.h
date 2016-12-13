#ifndef SN2OR_H
#define SN2OR_H

/// @file Sn2Or.h
/// @brief Sn2Or のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Gate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class Sn2Or Sn2Or.h
/// @brief ORノード
//////////////////////////////////////////////////////////////////////
class Sn2Or :
  public Sn2Gate
{
public:

  /// @brief コンストラクタ
  Sn2Or(ymuint id,
       const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Or();


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
/// @class Sn2Or2 Sn2Or.h
/// @brief 2入力ORノード
//////////////////////////////////////////////////////////////////////
class Sn2Or2 :
  public Sn2Gate2
{
public:

  /// @brief コンストラクタ
  Sn2Or2(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Or2();


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
/// @class Sn2Or3 Sn2Or.h
/// @brief 3入力ORノード
//////////////////////////////////////////////////////////////////////
class Sn2Or3 :
  public Sn2Gate3
{
public:

  /// @brief コンストラクタ
  Sn2Or3(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Or3();


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
/// @class Sn2Or4 Sn2Or.h
/// @brief 4入力ORノード
//////////////////////////////////////////////////////////////////////
class Sn2Or4 :
  public Sn2Gate4
{
public:

  /// @brief コンストラクタ
  Sn2Or4(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Or4();


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
/// @class Sn2Nor Sn2Or.h
/// @brief NORノード
//////////////////////////////////////////////////////////////////////
class Sn2Nor :
  public Sn2Or
{
public:

  /// @brief コンストラクタ
  Sn2Nor(ymuint id,
	 const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nor();


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
/// @class Sn2Nor2 Sn2Or.h
/// @brief 2入力NORノード
//////////////////////////////////////////////////////////////////////
class Sn2Nor2 :
  public Sn2Or2
{
public:

  /// @brief コンストラクタ
  Sn2Nor2(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nor2();


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
/// @class Sn2Nor3 Sn2Or.h
/// @brief 3入力NORノード
//////////////////////////////////////////////////////////////////////
class Sn2Nor3 :
  public Sn2Or3
{
public:

  /// @brief コンストラクタ
  Sn2Nor3(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nor3();


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
/// @class Sn2Nor2 Sn2Or.h
/// @brief 2入力NORノード
//////////////////////////////////////////////////////////////////////
class Sn2Nor4 :
  public Sn2Or4
{
public:

  /// @brief コンストラクタ
  Sn2Nor4(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Nor4();


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

#endif // SN2OR_H

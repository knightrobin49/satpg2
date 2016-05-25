#ifndef SNAND_H
#define SNAND_H

/// @file SnAnd.h
/// @brief SnAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_TFSIM2

//////////////////////////////////////////////////////////////////////
/// @class SnAnd SimNode.h
/// @brief ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnAnd(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd();


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

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd2 SimNode.h
/// @brief 2入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  SnAnd2(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd2();


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

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd3 SimNode.h
/// @brief 3入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd3 :
  public SnGate3
{
public:

  /// @brief コンストラクタ
  SnAnd3(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd3();


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

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd4 SimNode.h
/// @brief 4入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd4 :
  public SnGate4
{
public:

  /// @brief コンストラクタ
  SnAnd4(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd4();


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

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand SimNode.h
/// @brief NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand :
  public SnAnd
{
public:

  /// @brief コンストラクタ
  SnNand(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand();


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


//////////////////////////////////////////////////////////////////////
/// @class SnNand2 SimNode.h
/// @brief 2入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand2 :
  public SnAnd2
{
public:

  /// @brief コンストラクタ
  SnNand2(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand2();


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


//////////////////////////////////////////////////////////////////////
/// @class SnNand3 SimNode.h
/// @brief 3入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand3 :
  public SnAnd3
{
public:

  /// @brief コンストラクタ
  SnNand3(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand3();


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


//////////////////////////////////////////////////////////////////////
/// @class SnNand4 SimNode.h
/// @brief 4入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand4 :
  public SnAnd4
{
public:

  /// @brief コンストラクタ
  SnNand4(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand4();


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

#endif // SNAND_H

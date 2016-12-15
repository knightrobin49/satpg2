#ifndef SNXOR_H
#define SNXOR_H

/// @file SnXor.h
/// @brief SnXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnXor SimNode.h
/// @brief XORノード
//////////////////////////////////////////////////////////////////////
class SnXor :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnXor(ymuint id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXor();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  PackedVal3
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_fval3();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_gobs3(ymuint ipos);

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor2 SimNode.h
/// @brief 2入力XORノード
//////////////////////////////////////////////////////////////////////
class SnXor2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  SnXor2(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXor2();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  PackedVal3
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_fval3();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_gobs3(ymuint ipos);

};


//////////////////////////////////////////////////////////////////////
/// @class SnXnor SimNode.h
/// @brief XNORノード
//////////////////////////////////////////////////////////////////////
class SnXnor :
  public SnXor
{
public:

  /// @brief コンストラクタ
  SnXnor(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXnor();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  PackedVal3
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_fval3();

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor2 SimNode.h
/// @brief 2入力XNORノード
//////////////////////////////////////////////////////////////////////
class SnXnor2 :
  public SnXor2
{
public:

  /// @brief コンストラクタ
  SnXnor2(ymuint id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXnor2();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  PackedVal3
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_fval3();

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNXOR_H

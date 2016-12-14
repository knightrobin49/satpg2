#ifndef SNAND_H
#define SNAND_H

/// @file SnAnd.h
/// @brief SnAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnAnd SimNode.h
/// @brief ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnAnd(ymuint id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd();


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
  _calc_lobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_lobs3(ymuint ipos);

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
  SnAnd2(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd2();


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
  _calc_lobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_lobs3(ymuint ipos);

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
  SnAnd3(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd3();


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
  _calc_lobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_lobs3(ymuint ipos);

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
  SnAnd4(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd4();


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
  _calc_lobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_lobs3(ymuint ipos);

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
  SnNand(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand();


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
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

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
  SnNand2(ymuint id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand2();


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
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

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
  SnNand3(ymuint id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand3();


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
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

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
  SnNand4(ymuint id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand4();


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
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNAND_H

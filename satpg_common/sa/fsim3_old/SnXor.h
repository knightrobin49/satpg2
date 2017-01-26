#ifndef SNXOR_H
#define SNXOR_H

/// @file SnXor.h
/// @brief SnXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
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
  SnXor(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXor();


public:

  /// @brief 正常値の計算を行う．
  ///
  /// 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval();

  /// @brief 故障値の計算を行う．
  /// @param[in] mask マスク
  ///
  /// 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

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
  SnXor2(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXor2();


public:

  /// @brief 正常値の計算を行う．
  ///
  /// 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval();

  /// @brief 故障値の計算を行う．
  /// @param[in] mask マスク
  ///
  /// 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．
  virtual
  PackedVal
  calc_gobs(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

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
  SnXnor(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXnor();


public:

  /// @brief 正常値の計算を行う．
  ///
  /// 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval();

  /// @brief 故障値の計算を行う．
  /// @param[in] mask マスク
  ///
  /// 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

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
  SnXnor2(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnXnor2();


public:

  /// @brief 正常値の計算を行う．
  ///
  /// 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval();

  /// @brief 故障値の計算を行う．
  /// @param[in] mask マスク
  ///
  /// 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNXOR_H

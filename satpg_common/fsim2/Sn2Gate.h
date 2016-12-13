#ifndef SN2GATE_H
#define SN2GATE_H

/// @file Sn2Gate.h
/// @brief Sn2Gate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode2.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class Sn2Gate Sn2Gate.h
/// @brief 多入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class Sn2Gate :
  public SimNode2
{
protected:

  /// @brief コンストラクタ
  Sn2Gate(ymuint id,
	  const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Gate();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode2*
  fanin(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス用の関数
  //
  // これがある理由は virtual 関数はインライン展開できないから．
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  ymuint
  _fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  SimNode2*
  _fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint mFaninNum;

  // ファンインの配列
  SimNode2** mFanins;

};


//////////////////////////////////////////////////////////////////////
/// @class Sn2Gate1 Sn2Gate.h
/// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class Sn2Gate1 :
  public SimNode2
{
protected:

  /// @brief コンストラクタ
  Sn2Gate1(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Gate1();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode2*
  fanin(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス用の関数
  //
  // これがある理由は virtual 関数はインライン展開できないから．
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番めのファンインを得る．
  SimNode2*
  _fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンイン
  SimNode2* mFanin;

};


//////////////////////////////////////////////////////////////////////
/// @class Sn2Gate2 Sn2Gate.h
/// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class Sn2Gate2 :
  public SimNode2
{
protected:

  /// @brief コンストラクタ
  Sn2Gate2(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Gate2();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode2*
  fanin(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス用の関数
  //
  // これがある理由は virtual 関数はインライン展開できないから．
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番めのファンインを得る．
  SimNode2*
  _fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインの配列
  SimNode2* mFanins[2];

};


//////////////////////////////////////////////////////////////////////
/// @class Sn2Gate3 Sn2Gate.h
/// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class Sn2Gate3 :
  public SimNode2
{
protected:

  /// @brief コンストラクタ
  Sn2Gate3(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Gate3();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode2*
  fanin(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス用の関数
  //
  // これがある理由は virtual 関数はインライン展開できないから．
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番めのファンインを得る．
  SimNode2*
  _fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインの配列
  SimNode2* mFanins[3];

};


//////////////////////////////////////////////////////////////////////
/// @class Sn2Gate4 Sn2Gate.h
/// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class Sn2Gate4 :
  public SimNode2
{
protected:

  /// @brief コンストラクタ
  Sn2Gate4(ymuint id,
	   const vector<SimNode2*>& inputs);

  /// @brief デストラクタ
  virtual
  ~Sn2Gate4();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode2*
  fanin(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス用の関数
  //
  // これがある理由は virtual 関数はインライン展開できないから．
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番めのファンインを得る．
  SimNode2*
  _fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインの配列
  SimNode2* mFanins[4];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファンイン数を得る．
inline
ymuint
Sn2Gate::_fanin_num() const
{
  return mFaninNum;
}

// @brief pos 番めのファンインを得る．
inline
SimNode2*
Sn2Gate::_fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief pos 番めのファンインを得る．
inline
SimNode2*
Sn2Gate1::_fanin(ymuint pos) const
{
  return mFanin;
}

// @brief pos 番めのファンインを得る．
inline
SimNode2*
Sn2Gate2::_fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief pos 番めのファンインを得る．
inline
SimNode2*
Sn2Gate3::_fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief pos 番めのファンインを得る．
inline
SimNode2*
Sn2Gate4::_fanin(ymuint pos) const
{
  return mFanins[pos];
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // SN2GATE_H

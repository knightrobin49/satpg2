#ifndef TPGLOGICAND_H
#define TPGLOGICAND_H

/// @file TpgLogicAND.h
/// @brief TpgLogicAND[x] のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic2.h"
#include "TpgLogic3.h"
#include "TpgLogic4.h"
#include "TpgLogicN.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogicAND2 TpgLogicAND.h "TpgLogicAND.h"
/// @brief 2入力ANDを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicAND2 :
  public TpgLogic2
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] inode0, inode1 ファンインのノード
  TpgLogicAND2(ymuint id,
	       const char* name,
	       TpgNode* inode0,
	       TpgNode* inode1);

  /// @brief デストラクタ
  ~TpgLogicAND2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fpos 故障のある入力位置
  /// @param[in] fval 故障値 ( 0 / 1 )
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  ///
  /// こちらは入力に故障を仮定したバージョン
  virtual
  void
  make_faulty_cnf(SatSolver& solver,
		  ymuint fpos,
		  int fval,
		  const LitMap& lit_map) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
/// @class TpgLogicAND3 TpgLogicAND.h "TpgLogicAND.h"
/// @brief 3入力ANDを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicAND3 :
  public TpgLogic3
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] inode0, inode1, inode2 ファンインのノード
  TpgLogicAND3(ymuint id,
	       const char* name,
	       TpgNode* inode0,
	       TpgNode* inode1,
	       TpgNode* inode2);

  /// @brief デストラクタ
  ~TpgLogicAND3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fpos 故障のある入力位置
  /// @param[in] fval 故障値 ( 0 / 1 )
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  ///
  /// こちらは入力に故障を仮定したバージョン
  virtual
  void
  make_faulty_cnf(SatSolver& solver,
		  ymuint fpos,
		  int fval,
		  const LitMap& lit_map) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
/// @class TpgLogicAND4 TpgLogicAND.h "TpgLogicAND.h"
/// @brief 4入力ANDを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicAND4 :
  public TpgLogic4
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] inode0, inode1, inode2, inode3 ファンインのノード
  TpgLogicAND4(ymuint id,
	       const char* name,
	       TpgNode* inode0,
	       TpgNode* inode1,
	       TpgNode* inode2,
	       TpgNode* inode3);

  /// @brief デストラクタ
  ~TpgLogicAND4();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fpos 故障のある入力位置
  /// @param[in] fval 故障値 ( 0 / 1 )
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  ///
  /// こちらは入力に故障を仮定したバージョン
  virtual
  void
  make_faulty_cnf(SatSolver& solver,
		  ymuint fpos,
		  int fval,
		  const LitMap& lit_map) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
/// @class TpgLogicANDN TpgLogicAND.h "TpgLogicAND.h"
/// @brief N入力ANDを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicANDN :
  public TpgLogicN
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] fanin_num ファンイン数
  /// @param[in] fanin_array ファンインの配列
  /// @param[in] fault_array 入力の故障の配列
  TpgLogicANDN(ymuint id,
	       const char* name,
	       ymuint fanin_num,
	       TpgNode** fanin_array,
	       TpgFault** fault_array);

  /// @brief デストラクタ
  ~TpgLogicANDN();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fpos 故障のある入力位置
  /// @param[in] fval 故障値 ( 0 / 1 )
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  ///
  /// こちらは入力に故障を仮定したバージョン
  virtual
  void
  make_faulty_cnf(SatSolver& solver,
		  ymuint fpos,
		  int fval,
		  const LitMap& lit_map) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_SATPG

#endif // TPGLOGICAND_H

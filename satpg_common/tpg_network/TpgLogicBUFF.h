#ifndef TPGLOGICBUFF_H
#define TPGLOGICBUFF_H

/// @file TpgLogicBUFF.h
/// @brief TpgLogicBUFF のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogicBUFF TpgLogicBUFF.h "TpgLogicBUFF.h"
/// @brief buffer を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicBUFF :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] fanin_list ファンインのリスト
  /// @param[in] fanout_num ファンアウト数
  TpgLogicBUFF(ymuint id,
	       const vector<TpgNode*>& fanin_list,
	       ymuint fanout_num);

  /// @brief デストラクタ
  ~TpgLogicBUFF();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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

#endif // TPGLOGICBUFF_H

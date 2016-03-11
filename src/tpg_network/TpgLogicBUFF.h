#ifndef TPGLOGICBUFF_H
#define TPGLOGICBUFF_H

/// @file TpgLogicBUFF.h
/// @brief TpgLogicBUFF のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic1.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogicBUFF TpgLogicBUFF.h "TpgLogicBUFF.h"
/// @brief buffer を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgLogicBUFF :
  public TpgLogic1
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] inode ファンインのノード
  TpgLogicBUFF(ymuint id,
	       const char* name,
	       TpgNode* inode);

  /// @brief デストラクタ
  ~TpgLogicBUFF();


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

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
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

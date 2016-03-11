#ifndef TPGINPUT_H
#define TPGINPUT_H

/// @file TpgInput.h
/// @brief TpgInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgInput TpgInput.h "TpgInput.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgInput :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] input_id 入力番号
  TpgInput(ymuint id,
	   const char* name,
	   ymuint input_id);

  /// @brief デストラクタ
  virtual
  ~TpgInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  virtual
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ///
  /// node = TpgNetwork::input(node->input_id()
  /// の関係を満たす．
  /// is_input() が false の場合の返り値は不定
  virtual
  ymuint
  input_id() const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  const TpgFault*
  output_fault(int val) const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  TpgFault*
  output_fault(int val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] fault 故障
  virtual
  void
  set_output_fault(int val,
		   TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号
  ymuint mInputId;

  // 故障
  TpgFault* mFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGINPUT_H

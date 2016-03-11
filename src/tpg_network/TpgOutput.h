#ifndef TPGOUTPUT_H
#define TPGOUTPUT_H

/// @file TpgOutput.h
/// @brief TpgOutput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgOutput TpgOutput.h "TpgOutput.h"
/// @brief 出力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgOutput :
  public TpgNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] output_id 出力番号
  /// @param[in] fanin ファンインのノード
  TpgOutput(ymuint id,
	    const char* name,
	    ymuint output_id,
	    TpgNode* fanin);

  /// @brief デストラクタ
  ~TpgOutput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  virtual
  bool
  is_output() const;

  /// @brief 外部出力タイプの時に出力番号を返す．
  ///
  /// node = TpgNetwork::output(node->output_id())
  /// の関係を満たす．
  /// is_output() が false の場合の返り値は不定
  virtual
  ymuint
  output_id() const;

  /// @brief TFIサイズの昇順に並べた時の出力番号を返す．
  virtual
  ymuint
  output_id2() const;

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  TpgFault*
  input_fault(int val,
	      ymuint pos);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力番号2をセットする．
  /// @param[in] id セットする番号
  ///
  /// 出力ノード以外では無効
  virtual
  void
  set_output_id2(ymuint id);

  /// @brief 入力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  /// @param[in] fault 故障
  virtual
  void
  set_input_fault(int val,
		  ymuint pos,
		  TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力番号
  ymuint mOutputId;

  // 出力番号2
  ymuint mOutputId2;

  // ファンイン
  TpgNode* mFanin;

  // 故障
  TpgFault* mFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGOUTPUT_H

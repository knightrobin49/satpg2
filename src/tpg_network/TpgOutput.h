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
  /// @param[in] output_id 出力番号
  /// @param[in] fanin ファンインのノード
  TpgOutput(ymuint id,
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

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;

  /// @brief このノードに関係する故障数を返す．
  virtual
  ymuint
  fault_num() const;

  /// @brief このノードに関係する故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  virtual
  const TpgFault*
  fault(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力番号
  ymuint32 mOutputId;

  // 出力番号2
  ymuint32 mOutputId2;

  // ファンイン
  TpgNode* mFanin;

  // 故障
  TpgFault* mFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGOUTPUT_H

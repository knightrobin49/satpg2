#ifndef DTPGSATS0_H
#define DTPGSATS0_H

/// @file DtpgSatS0.h
/// @brief DtpgSatS0 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatS0 DtpgSatS0.h "DtpgSatS0.h"
/// @brief 1つの故障を対象とした CNF を生成する DtpgSat
//////////////////////////////////////////////////////////////////////
class DtpgSatS0 :
  public DtpgSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatS0(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatS0();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fault_list 対象の故障リスト
  /// @param[out] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
      FaultMgr& fmgr,
      Fsim& fsim,
      const vector<const TpgFault*>& fault_list,
      DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] fault 対象の故障
  void
  run_single(const TpgFault* fault);

  /// @brief TFO マークを調べる．
  bool
  tfo_mark(const TpgNode* node) const;

  /// @brief TFO マークをつける．
  void
  set_tfo_mark(const TpgNode* node);

  /// @brief TFI マークを調べる．
  bool
  tfi_mark(const TpgNode* node) const;

  /// @brief TFI マークをつける．
  void
  set_tfi_mark(const TpgNode* node);

  /// @brief TFO マークと TFI マークのいづれかがついていたら true を返す．
  bool
  mark(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymuint mMaxNodeId;

  vector<const TpgNode*> mNodeList;

  vector<ymuint8> mMarkArray;

};

// @brief TFO マークを調べる．
inline
bool
DtpgSatS0::tfo_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// @brief TFO マークをつける．
inline
void
DtpgSatS0::set_tfo_mark(const TpgNode* node)
{
  ymuint id = node->id();
  if ( ((mMarkArray[id] >> 0) & 1U) == 0U ) {
    mMarkArray[id] = 1U;
    mNodeList.push_back(node);
  }
}

// @brief TFI マークを調べる．
inline
bool
DtpgSatS0::tfi_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief TFI マークをつける．
inline
void
DtpgSatS0::set_tfi_mark(const TpgNode* node)
{
  ymuint id = node->id();
  if ( mMarkArray[id] == 0U ) {
    mMarkArray[node->id()] = 2U;
    mNodeList.push_back(node);
  }
}

// @brief TFO マークと TFI マークのいづれかがついていたら true を返す．
inline
bool
DtpgSatS0::mark(const TpgNode* node)
{
  if ( mMarkArray[node->id()] ) {
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSATS0_H

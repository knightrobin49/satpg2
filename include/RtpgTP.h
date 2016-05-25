#ifndef RTPGTP_H
#define RTPGTP_H

/// @file RtpgTP.h
/// @brief RtpgTP のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgTP RtpgTP.h "RtpgTP.h"
/// @brief RTPG を行う基底クラス
//////////////////////////////////////////////////////////////////////
class RtpgTP
{
public:

  /// @brief デストラクタ
  virtual
  ~RtpgTP() {}

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数生成器を初期化する．
  /// @param[in] seed 乱数の種
  virtual
  void
  init(ymuint32 seed) = 0;

  /// @brief RTPGを行なう．
  /// @param[in] fault_list 故障のリスト
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  /// @param[in] wsa_limit WSA の制限値
  /// @param[out] det_fault_list 検出された故障のリスト
  /// @param[out] tvlist テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(const vector<const TpgFault*>& fault_list,
      TvMgr& tvmgr,
      Fsim& fsim,
      ymuint min_f,
      ymuint max_i,
      ymuint max_pat,
      ymuint wsa_limit,
      vector<const TpgFault*>& det_fault_list,
      vector<TestVector*>& tvlist,
      RtpgStats& stats) = 0;

};


/// @brief RtpgTP のインスタンスを生成する．
extern
RtpgTP*
new_RtpgTP1();

/// @brief RtpgTP のインスタンスを生成する．
extern
RtpgTP*
new_RtpgTP2(ymuint nbits = 1);

END_NAMESPACE_YM_SATPG

#endif // RTPGTP_H

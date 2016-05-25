#ifndef RTPGTP1_H
#define RTPGTP1_H

/// @file RtpgTP1.h
/// @brief RtpgTP1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgTP.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgTP1 RtpgTP1.h "RtpgTP1.h"
/// @brief Fsim を使う Rtpg
//////////////////////////////////////////////////////////////////////
class RtpgTP1 :
  public RtpgTP
{
public:

  /// @brief コンストラクタ
  RtpgTP1();

  /// @brief デストラクタ
  virtual
  ~RtpgTP1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数生成器を初期化する．
  /// @param[in] seed 乱数の種
  virtual
  void
  init(ymuint32 seed);

  /// @brief RTPGを行なう．
  /// @param[in] fault_list 故障リスト
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
      RtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数生成器
  RandGen mRandGen;

  // 制限値
  ymuint mLimit;

};

END_NAMESPACE_YM_SATPG

#endif // RTPGTP1_H

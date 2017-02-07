#ifndef DTPGF_H
#define DTPGF_H

/// @file DtpgF.h
/// @brief DtpgF のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgBase.h"
#include "sa/StructSat.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgF DtpgF.h "DtpgF.h"
/// @brief FFR の構造を考慮した階層的な DtpgBase
//////////////////////////////////////////////////////////////////////
class DtpgF :
  public DtpgBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] network 対象のネットワーク
  /// @param[in] mffc_root MFFC の根のノード
  DtpgF(const string& sat_type,
	const string& sat_option,
	ostream* sat_outp,
	BackTracer& bt,
	const TpgNetwork& network,
	const TpgNode* mffc_root);

  /// @brief デストラクタ
  virtual
  ~DtpgF();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の故障数を返す．
  ymuint
  fault_num() const;

  /// @brief 対象の故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  fault(ymuint pos) const;

  /// @brief CNF 式を作る．
  void
  cnf_gen(DtpgStats& stats);

  /// @brief テスト生成を行う．
  /// @param[in] fault 対象の故障
  /// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
  /// @param[inout] stats DTPGの統計情報
  /// @return 結果を返す．
  SatBool3
  dtpg(const TpgFault* fault,
       NodeValList& nodeval_list,
       DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief node と同じ FFR 内の故障を mFaultList に入れる．
  /// @param[in] node 対象のノード
  void
  get_ffr_faults(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TpgNetwork& mNetwork;

  // FFR の根のノード
  const TpgNode* mFfrRoot;

  // 対象の故障リスト
  vector<const TpgFault*> mFaultList;

  // テスト生成用のCNFを生成するためのクラス
  StructSat mStructSat;

  // FFR 用のデータ構造
  const FoCone* mFoCone;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGF_H

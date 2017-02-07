#ifndef DTPGM_H
#define DTPGM_H

/// @file DtpgM.h
/// @brief DtpgM のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgBase.h"
#include "sa/StructSat.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgM DtpgM.h "DtpgM.h"
/// @brief MFFC と FFR の構造を考慮した階層的な DtpgBase
//////////////////////////////////////////////////////////////////////
class DtpgM :
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
  DtpgM(const string& sat_type,
	const string& sat_option,
	ostream* sat_outp,
	BackTracer& bt,
	const TpgNetwork& network,
	const TpgNode* mffc_root);

  /// @brief デストラクタ
  virtual
  ~DtpgM();


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

  /// @briefテスト生成を行う．
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
  /// @param[in] elem_pos MFFC 内の要素番号
  void
  get_ffr_faults(const TpgNode* node,
		 ymuint elem_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障に関する情報を表す構造体
  struct FaultInfo
  {
    // 空のコンストラクタ
    FaultInfo() { }

    // FFR の根のノード
    const TpgNode* mFfrRoot;

    // MFFC 内の要素番号
    ymuint mElemPos;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TpgNetwork& mNetwork;

  // MFFC の根のノード
  const TpgNode* mMffcRoot;

  // 対象の故障リスト
  vector<const TpgFault*> mFaultList;

  // 故障番号をキーにして FaultInfo を保持する配列
  vector<FaultInfo> mFaultInfoArray;

  // テスト生成用のCNFを生成するためのクラス
  StructSat mStructSat;

  // MFFC 用のデータ構造
  const MffcCone* mMffcCone;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGM_H

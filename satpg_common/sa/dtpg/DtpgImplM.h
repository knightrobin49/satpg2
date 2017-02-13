#ifndef DTPGIMPLM_H
#define DTPGIMPLM_H

/// @file DtpgImplM.h
/// @brief DtpgImplM のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgImpl.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgImplM DtpgImplM.h "DtpgImplM.h"
/// @brief MFFC と FFR の構造を考慮した階層的な DtpgImpl
//////////////////////////////////////////////////////////////////////
class DtpgImplM :
  public DtpgImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] network 対象のネットワーク
  /// @param[in] mffc_root MFFC の根のノード
  DtpgImplM(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    BackTracer& bt,
	    const TpgNetwork& network,
	    const TpgNode* mffc_root);

  /// @brief デストラクタ
  virtual
  ~DtpgImplM();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CNF 式を作る．
  /// @param[out] stats DTPGの統計情報
  virtual
  void
  gen_cnf(DtpgStats& stats);

  /// @briefテスト生成を行う．
  /// @param[in] fault 対象の故障
  /// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
  /// @param[inout] stats DTPGの統計情報
  /// @return 結果を返す．
  virtual
  SatBool3
  dtpg(const TpgFault* fault,
       NodeValList& nodeval_list,
       DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MFFC 内部の故障伝搬条件を表すCNFを作る．
  void
  make_mffc_condition();

  /// @brief 故障挿入回路のCNFを作る．
  /// @param[in] elem_pos 要素番号
  /// @param[in] ovar ゲートの出力の変数
  void
  inject_fault(ymuint elem_pos,
	       SatVarId ovar);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FFR の根のリスト
  // [0] は MFFC の根でもある．
  vector<const TpgNode*> mElemArray;

  // 故障番号をキーにしてFFR番号を入れる配列
  vector<ymuint> mElemPosMap;

  // 各FFRの根に反転イベントを挿入するための変数
  // サイズは mElemNum
  vector<SatVarId> mElemVarArray;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGIMPLM_H

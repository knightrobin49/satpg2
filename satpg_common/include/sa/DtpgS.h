#ifndef DTPGS_H
#define DTPGS_H

/// @file DtpgS.h
/// @brief DtpgS のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DtpgBase.h"
#include "GenVidMap.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgS DtpgS.h "DtpgS.h"
/// @brief 1つの故障を対象とした CNF を生成する DtpgSat
//////////////////////////////////////////////////////////////////////
class DtpgS :
  public DtpgBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] network 対象のネットワーク
  /// @param[in] ffr_root 対象の故障のあるFFRの根のノード
  DtpgS(const string& sat_type,
	const string& sat_option,
	ostream* sat_outp,
	BackTracer& bt,
	const TpgNetwork& network,
	const TpgNode* ffr_root);

  /// @brief デストラクタ
  virtual
  ~DtpgS();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 回路の構造を表すCNF式を作る．
  /// @param[out] stats DTPGの統計情報
  ///
  /// このオブジェクトに対しては1回行えばよい．
  /// というか1回しか行えない．
  void
  gen_cnf(DtpgStats& stats);

  /// @brief テスト生成を行なう．
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

  /// @brief TFO マークを調べる．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(const TpgNode* node) const;

  /// @brief TFO マークをつける．
  /// @param[in] node 対象のノード
  ///
  /// と同時に mNodeList に入れる．<br>
  /// 出力ノードの場合は mOutputList にも入れる．<br>
  /// すでにマークされていたら何もしない．
  void
  set_tfo_mark(const TpgNode* node);

  /// @brief TFI マークを調べる．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(const TpgNode* node) const;

  /// @brief TFI マークをつける．
  /// @param[in] node 対象のノード
  ///
  /// と同時に mNodeList に入れる．
  void
  set_tfi_mark(const TpgNode* node);

  /// @brief TFO マークと TFI マークのいづれかがついていたら true を返す．
  /// @param[in] node 対象のノード
  bool
  mark(const TpgNode* node);

  /// @brief 故障伝搬条件を表すCNF式を生成する．
  /// @param[in] node 対象のノード
  void
  make_dchain_cnf(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver mSolver;

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 関係するノードを入れておくリスト
  vector<const TpgNode*> mNodeList;

  // 関係する出力ノードを入れておくリスト
  vector<const TpgNode*> mOutputList;

  // 作業用のマークを入れておく配列
  // サイズは mMaxNodeId
  vector<ymuint8> mMarkArray;

  // 対象のFFRの根のノード
  const TpgNode* mFfrRoot;

  // 正常値を表す変数のマップ
  GenVidMap mGvarMap;

  // 故障値を表す変数のマップ
  GenVidMap mFvarMap;

  // 故障伝搬条件を表す変数のマップ
  GenVidMap mDvarMap;

  // CNF が生成されている時 true となるフラグ
  bool mHasCnf;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief TFO マークを調べる．
inline
bool
DtpgS::tfo_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// @brief TFO マークをつける．
inline
void
DtpgS::set_tfo_mark(const TpgNode* node)
{
  ymuint id = node->id();
  if ( ((mMarkArray[id] >> 0) & 1U) == 0U ) {
    mMarkArray[id] = 1U;
    mNodeList.push_back(node);
    if ( node->is_ppo() ) {
      mOutputList.push_back(node);
    }
  }
}

// @brief TFI マークを調べる．
inline
bool
DtpgS::tfi_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief TFI マークをつける．
inline
void
DtpgS::set_tfi_mark(const TpgNode* node)
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
DtpgS::mark(const TpgNode* node)
{
  if ( mMarkArray[node->id()] ) {
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGS_H

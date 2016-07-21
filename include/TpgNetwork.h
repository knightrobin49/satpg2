#ifndef TPGNETWORK_H
#define TPGNETWORK_H

/// @file TpgNetwork.h
/// @brief TpgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "Val3.h"
#include "TpgNode.h"
#include "ym/ym_bnet.h"
#include "ym/ym_cell.h"
#include "ym/ym_logic.h"
#include "ym/SimpleAlloc.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_SATPG

class TpgNodeInfo;

//////////////////////////////////////////////////////////////////////
/// @class TpgNetwork TpgNetwork.h "TpgNetwork.h"
/// @brief SATPG 用のネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgNetwork
{
public:
  //////////////////////////////////////////////////////////////////////
  // ファイルを読み込んでインスタンスを作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  TpgNetwork();

  /// @brief デストラクタ
  ~TpgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  ///
  /// node->id() == id となるノードを返す．
  const TpgNode*
  node(ymuint id) const;

  /// @brief 外部入力数を得る．
  ymuint
  input_num() const;

  /// @brief 外部入力数 + FF数を得る．
  ymuint
  input_num2() const;

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  TpgNode*
  input(ymuint pos) const;

  /// @brief 外部出力数を得る．
  ymuint
  output_num() const;

  /// @brief 外部出力数 + FF数を得る．
  ymuint
  output_num2() const;

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  TpgNode*
  output(ymuint pos) const;

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  TpgNode*
  output2(ymuint pos) const;

  /// @brief DFF数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFの出力に対応した擬似入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
  ///
  /// 実は input(pos + input_num()) と同じ．
  TpgNode*
  dff_output(ymuint pos) const;

  /// @brief DFFの入力に対応した擬似出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
  ///
  /// 実は output(pos + input_num()) と同じ．
  TpgNode*
  dff_input(ymuint pos) const;

  /// @brief 故障IDの最大値+1を返す．
  ymuint
  max_fault_id() const;

  /// @brief MFFC 数を返す．
  ymuint
  mffc_num() const;

  /// @brief FFR 数を返す．
  ymuint
  ffr_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // アクティブ領域に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] po_pos 出力番号
  void
  activate_po(ymuint po_pos);

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] po 出力ノード
  void
  activate_po(TpgNode* po);

  /// @brief 全てのノードをアクティブにする．
  void
  activate_all();

  /// @brief アクティブなノード数を得る．
  ymuint
  active_node_num() const;

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  const TpgNode*
  active_node(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @return 読み込みが成功したら true を返す．
  bool
  read_blif(const string& filename,
	    const CellLibrary* cell_library = nullptr);

  /// @brief iscas89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @return 読み込みが成功したら true を返す．
  bool
  read_iscas89(const string& filename);

  /// @brief BnBuilder から内容を設定する．
  /// @param[in] builder ビルダーオブジェクト
  void
  set(const BnBuilder& builder);

  /// @brief 入力ノードを生成する．
  /// @param[in] iid 入力の番号
  /// @param[in] name ノード名
  /// @return 生成したノードを返す．
  TpgNode*
  make_input_node(ymuint iid,
		  const string& name);

  /// @brief 出力ノードを生成する．
  /// @param[in] oid 出力の番号
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_output_node(ymuint oid,
		   const string& name,
		   TpgNode* inode);

  /// @brief 論理ノードを生成する．
  /// @param[in] name ノード名
  /// @param[in] node_info 論理関数の情報
  /// @param[in] fanin_list ファンインのリスト
  /// @return 生成したノードを返す．
  TpgNode*
  make_logic_node(const string& name,
		  const TpgNodeInfo* node_info,
		  const vector<TpgNode*>& fanin_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式から TpgNode の木を生成する．
  /// @param[in] name ノード名
  /// @param[in] expr 式
  /// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
  /// @param[in] input_map ファンインの対応関係を収める配列
  /// @return 生成したノードを返す．
  ///
  /// leaf_nodes は 変数番号 * 2 + (0/1) に
  /// 該当する変数の肯定/否定のリテラルが入っている．
  TpgNode*
  make_cplx_node(const string& name,
		 const Expr& expr,
		 const vector<TpgNode*>& leaf_nodes,
		 TpgNode** inode_array,
		 ymuint* ipos_array);

  /// @brief 組み込み型の論理ゲートを生成する．
  /// @param[in] name ノード名
  /// @param[in] type ゲートの型
  /// @param[in] fanin_list ファンインのリスト
  /// @return 生成したノードを返す．
  TpgNode*
  make_prim_node(const string& name,
		 GateType type,
		 const vector<TpgNode*>& fanin_list);

  /// @brief ファンアウト情報の設定を行う．
  void
  set_fanouts();

  /// @brief 出力の故障を作る．
  /// @param[in] name 故障位置のノード名
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] node 故障位置のノード
  const TpgFault*
  new_ofault(const string& name,
	     int val,
	     TpgNode* node);

  /// @brief 入力の故障を作る．
  /// @param[in] name 故障位置のノード名
  /// @param[in] ipos 故障位置のファンイン番号
  /// @param[in] node 故障位置のノード
  /// @param[in] inode_pos node 上の入力位置
  /// @param[in] val 故障値
  /// @param[in] rep 代表故障
  ///
  /// プリミティブ型の場合は ipos と inode_pos は同一だが
  /// 複合型の場合には異なる．
  const TpgFault*
  new_ifault(const string& name,
	     int val,
	     ymuint ipos,
	     TpgNode* node,
	     ymuint inode_pos,
	     const TpgFault* rep);

  /// @brief 代表故障を設定する．
  /// @param[in] node 対象のノード
  void
  set_rep_faults(TpgNode* node);

  /// @brief ノードの TFI にマークをつける．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  /// @note マークの追加ノードは mTmpNodeList[0] - mTmpNodeList[mTmpNodeNum - 1]
  /// に格納される．
  void
  tfimark(TpgNode* node);

  /// @brief TFI マークを消す．
  /// @note この関数が終了すると mTmpNodeNum は 0 になる．
  void
  clear_tfimark();

  /// @brief activate_po(), activate_all() の下請け関数
  void
  activate_sub();


private:
  //////////////////////////////////////////////////////////////////////
  // オブジェクトの生成はクラスメソッドのみが行なう．
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーコンストラクタは実装しない．
  TpgNetwork(const TpgNetwork& src);

  /// @brief 代入演算子も実装しない．
  const TpgNetwork&
  operator=(const TpgNetwork& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TpgNetwork 関係のメモリ確保を行なうオブジェクト
  SimpleAlloc mAlloc;

  // 外部入力数
  ymuint mInputNum;

  // 外部出力数
  ymuint mOutputNum;

  // FF数
  ymuint mFFNum;

  // ノード数(mNodeArrayの要素数)
  ymuint mNodeNum;

  // ノードの配列
  TpgNode** mNodeArray;

  // 外部入力ノードの配列
  TpgNode** mInputArray;

  // 外部出力ノードの配列
  TpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  TpgNode** mOutputArray2;

  // アクティブなノード数
  ymuint mActNodeNum;

  // アクティブなノードの配列
  TpgNode** mActNodeArray;

  // activate_sub() で用いられるマーク用の配列
  // サイズは mNodeNum
  bool* mTmpMark;

  // 一時的に用いるノードリスト
  // サイズは mNodeNum
  TpgNode** mTmpNodeList;

  // mTmpNodeList の見かけのサイズ
  ymuint mTmpNodeNum;

  // 全故障数
  ymuint mFaultNum;

  // MFFC 数
  ymuint mMffcNum;

  // FFR 数
  ymuint mFfrNum;

};

/// @brief TpgNetwork の内容を出力する関数
/// @param[in] s 出力先のストリーム
/// @param[in] network 対象のネットワーク
void
print_network(ostream& s,
	      const TpgNetwork& network);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
TpgNetwork::node_num() const
{
  return mNodeNum;
}

// @brief 外部入力数を得る．
inline
ymuint
TpgNetwork::input_num() const
{
  return mInputNum;
}

// @brief 外部入力数 + FF数を得る．
inline
ymuint
TpgNetwork::input_num2() const
{
  return mInputNum + mFFNum;
}

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
inline
TpgNode*
TpgNetwork::input(ymuint pos) const
{
  ASSERT_COND( pos < input_num2() );
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
TpgNetwork::output_num() const
{
  return mOutputNum;
}

// @brief 外部出力数 + FF数を得る．
inline
ymuint
TpgNetwork::output_num2() const
{
  return mOutputNum + mFFNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
inline
TpgNode*
TpgNetwork::output(ymuint pos) const
{
  ASSERT_COND( pos < output_num2() );
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
TpgNode*
TpgNetwork::output2(ymuint pos) const
{
  ASSERT_COND( pos < output_num2() );
  return mOutputArray2[pos];
}

// @brief DFF数を得る．
inline
ymuint
TpgNetwork::dff_num() const
{
  return mFFNum;
}

// @brief DFFの出力に対応した擬似入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
//
// 実は input(pos + input_num()) と同じ．
inline
TpgNode*
TpgNetwork::dff_output(ymuint pos) const
{
  ASSERT_COND( pos < dff_num() );
  return mInputArray[pos + input_num()];
}

// @brief DFFの入力に対応した擬似出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
//
// 実は output(pos + input_num()) と同じ．
inline
TpgNode*
TpgNetwork::dff_input(ymuint pos) const
{
  ASSERT_COND( pos < dff_num() );
  return mOutputArray[pos + output_num()];
}

// @brief ノードを得る．
inline
const TpgNode*
TpgNetwork::node(ymuint pos) const
{
  ASSERT_COND( pos < mNodeNum );
  return mNodeArray[pos];
}

// @brief 故障IDの最大値+1を返す．
inline
ymuint
TpgNetwork::max_fault_id() const
{
  return mFaultNum;
}

// @brief アクティブなノード数を得る．
inline
ymuint
TpgNetwork::active_node_num() const
{
  return mActNodeNum;
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
const TpgNode*
TpgNetwork::active_node(ymuint pos) const
{
  ASSERT_COND( pos < mActNodeNum );
  return mActNodeArray[pos];
}

// @brief MFFC 数を返す．
inline
ymuint
TpgNetwork::mffc_num() const
{
  return mMffcNum;
}

// @brief FFR 数を返す．
inline
ymuint
TpgNetwork::ffr_num() const
{
  return mFfrNum;
}

END_NAMESPACE_YM_SATPG

#endif // TPGNETWORK_H

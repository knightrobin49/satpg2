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
#include "TpgDff.h"
#include "TpgNode.h"

#include "ym/ym_bnet.h"
#include "ym/ym_cell.h"
#include "ym/ym_logic.h"
#include "ym/SimpleAlloc.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_SATPG

class TpgNodeInfo;
class AuxNodeInfo;

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

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  TpgNode*
  input(ymuint pos) const;

  /// @brief 外部出力数を得る．
  ymuint
  output_num() const;

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
  TpgNode*
  output(ymuint pos) const;

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  TpgNode*
  output2(ymuint pos) const;

  /// @brief DFF数を得る．
  ymuint
  dff_num() const;

  /// @brief DFF を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
  const TpgDff*
  dff(ymuint pos) const;

  /// @brief スキャン方式の擬似外部入力数を得る．
  ///
  /// = input_num() + dff_num()
  ymuint
  pseudo_input_num() const;

  /// @brief スキャン方式の擬似外部入力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < pseudo_input_num() )
  TpgNode*
  pseudo_input(ymuint pos) const;

  /// @brief スキャン方式の擬似外部出力数を得る．
  ///
  /// = output_num() + dff_num()
  ymuint
  pseudo_output_num() const;

  /// @brief スキャン方式の擬似外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < pseudo_output_num() )
  TpgNode*
  pseudo_output(ymuint pos) const;

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
  // ノードに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード名を返す．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  const char*
  node_name(ymuint id) const;

  /// @brief 出力の故障を得る．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  /// @param[in] val 故障値 ( 0 / 1 )
  TpgFault*
  _node_output_fault(ymuint id,
		     int val);

  /// @brief 入力の故障を得る．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  TpgFault*
  _node_input_fault(ymuint id,
		    int val,
		    ymuint pos);

  /// @brief 出力の故障を得る．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  /// @param[in] val 故障値 ( 0 / 1 )
  const TpgFault*
  node_output_fault(ymuint id,
		    int val) const;

  /// @brief 入力の故障を得る．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  const TpgFault*
  node_input_fault(ymuint id,
		   int val,
		   ymuint pos) const;

  /// @brief このノードに関係する代表故障数を返す．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  ymuint
  node_fault_num(ymuint id) const;

  /// @brief このノードに関係する代表故障を返す．
  /// @param[in] id ノードID ( 0 <= id < node_num() )
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  node_fault(ymuint id,
	     ymuint pos) const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 複合ゲートの入力の情報
  ///
  /// もとの入力が実際の TpgNode のどのファンインに
  /// 対応しているかを表す．
  struct InodeInfo
  {
    InodeInfo(TpgNode* node = nullptr,
	      ymuint pos = 0) :
      mNode(node),
      mPos(pos)
    {
    }

    void
    set(TpgNode* node,
	ymuint pos)
    {
      mNode = node;
      mPos = pos;
    }

    TpgNode* mNode;
    ymuint mPos;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief BnNetwork から内容を設定する．
  /// @param[in] network 設定元のネットワーク
  void
  set(const BnNetwork& network);

  /// @brief 入力ノードを生成する．
  /// @param[in] iid 入力の番号
  /// @param[in] name ノード名
  /// @param[in] fanout_num ファンアウト数
  /// @return 生成したノードを返す．
  TpgNode*
  make_input_node(ymuint iid,
		  const string& name,
		  ymuint fanout_num);

  /// @brief 出力ノードを生成する．
  /// @param[in] oid 出力の番号
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_output_node(ymuint oid,
		   const string& name,
		   TpgNode* inode);

  /// @brief DFFの入力ノードを生成する．
  /// @param[in] oid 出力の番号
  /// @param[in] dff 接続しているDFF
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_dff_input_node(ymuint oid,
		      TpgDff* dff,
		      const string& name,
		      TpgNode* inode);

  /// @brief DFFの出力ノードを生成する．
  /// @param[in] iid 入力の番号
  /// @param[in] dff 接続しているDFF
  /// @param[in] name ノード名
  /// @param[in] fanout_num ファンアウト数
  /// @return 生成したノードを返す．
  TpgNode*
  make_dff_output_node(ymuint iid,
		       TpgDff* dff,
		       const string& name,
		       ymuint fanout_num);

  /// @brief DFFのクロック端子を生成する．
  /// @param[in] dff 接続しているDFF
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_dff_clock_node(TpgDff* dff,
		      const string& name,
		      TpgNode* inode);

  /// @brief DFFのクリア端子を生成する．
  /// @param[in] dff 接続しているDFF
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_dff_clear_node(TpgDff* dff,
		      const string& name,
		      TpgNode* inode);

  /// @brief DFFのプリセット端子を生成する．
  /// @param[in] dff 接続しているDFF
  /// @param[in] name ノード名
  /// @param[in] inode 入力のノード
  /// @return 生成したノードを返す．
  TpgNode*
  make_dff_preset_node(TpgDff* dff,
		       const string& name,
		       TpgNode* inode);

  /// @brief 論理ノードを生成する．
  /// @param[in] name ノード名
  /// @param[in] node_info 論理関数の情報
  /// @param[in] fanin_list ファンインのリスト
  /// @param[in] fanout_num ファンアウト数
  /// @return 生成したノードを返す．
  TpgNode*
  make_logic_node(const string& name,
		  const TpgNodeInfo* node_info,
		  const vector<TpgNode*>& fanin_list,
		  ymuint fanout_num);

  /// @brief 論理式から TpgNode の木を生成する．
  /// @param[in] name ノード名
  /// @param[in] expr 式
  /// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
  /// @param[in] inode_array ファンインの対応関係を収める配列
  /// @param[in] fanout_num ファンアウト数
  /// @return 生成したノードを返す．
  ///
  /// leaf_nodes は 変数番号 * 2 + (0/1) に
  /// 該当する変数の肯定/否定のリテラルが入っている．
  TpgNode*
  make_cplx_node(const string& name,
		 const Expr& expr,
		 const vector<TpgNode*>& leaf_nodes,
		 vector<InodeInfo>& inode_array,
		 ymuint fanout_num);

  /// @brief 組み込み型の論理ゲートを生成する．
  /// @param[in] name ノード名
  /// @param[in] type ゲートの型
  /// @param[in] fanin_list ファンインのリスト
  /// @param[in] fanout_num ファンアウト数
  /// @return 生成したノードを返す．
  TpgNode*
  make_prim_node(const string& name,
		 GateType type,
		 const vector<TpgNode*>& fanin_list,
		 ymuint fanout_num);

  /// @brief 出力の故障を作る．
  /// @param[in] name 故障位置のノード名
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] node 故障位置のノード
  void
  new_ofault(const char* name,
	     int val,
	     TpgNode* node);

  /// @brief 入力の故障を作る．
  /// @param[in] name 故障位置のノード名
  /// @param[in] ipos 故障位置のファンイン番号
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] inode_info TpgNode 上のノードの情報
  /// @param[in] rep 代表故障
  ///
  /// プリミティブ型の場合は ipos と inode_pos は同一だが
  /// 複合型の場合には異なる．
  void
  new_ifault(const char* name,
	     ymuint ipos,
	     int val,
	     const InodeInfo& inode_info,
	     const TpgFault* rep);

  /// @brief 代表故障を設定する．
  /// @param[in] node 対象のノード
  void
  set_rep_faults(TpgNode* node);


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

  // DFF数
  ymuint mDffNum;

  // DFFの実体の配列
  TpgDff* mDffArray;

  // ノード数(mNodeArrayの要素数)
  ymuint mNodeNum;

  // ノードの配列
  TpgNode** mNodeArray;

  // ノードの付加情報の配列
  AuxNodeInfo* mAuxInfoArray;

  // 外部入力ノードの配列
  TpgNode** mInputArray;

  // 外部出力ノードの配列
  TpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  TpgNode** mOutputArray2;

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

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
inline
TpgNode*
TpgNetwork::input(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
TpgNetwork::output_num() const
{
  return mOutputNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
inline
TpgNode*
TpgNetwork::output(ymuint pos) const
{
  ASSERT_COND( pos < output_num() );
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
TpgNode*
TpgNetwork::output2(ymuint pos) const
{
  ASSERT_COND( pos < output_num() );
  return mOutputArray2[pos];
}

// @brief DFF数を得る．
inline
ymuint
TpgNetwork::dff_num() const
{
  return mDffNum;
}

// @brief DFF を得る．
// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
inline
const TpgDff*
TpgNetwork::dff(ymuint pos) const
{
  ASSERT_COND( pos < dff_num() );
  return &mDffArray[pos];
}

// @brief スキャン方式の擬似外部入力数を得る．
//
// = input_num() + dff_num()
inline
ymuint
TpgNetwork::pseudo_input_num() const
{
  return mInputNum + mDffNum;
}

// @brief スキャン方式の擬似外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < pseudo_input_num() )
inline
TpgNode*
TpgNetwork::pseudo_input(ymuint pos) const
{
  ASSERT_COND( pos < pseudo_input_num() );
  return mInputArray[pos];
}

// @brief スキャン方式の擬似外部出力数を得る．
//
// = output_num() + dff_num()
inline
ymuint
TpgNetwork::pseudo_output_num() const
{
  return mOutputNum + mDffNum;
}

// @brief スキャン方式の擬似外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < pseudo_output_num() )
inline
TpgNode*
TpgNetwork::pseudo_output(ymuint pos) const
{
  ASSERT_COND( pos < pseudo_output_num() );
  return mOutputArray[pos];
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

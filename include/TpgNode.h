#ifndef TPGNODE_H
#define TPGNODE_H

/// @file TpgNode.h
/// @brief TpgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "LitMap.h"
#include "Val3.h"
#include "ym/Alloc.h"
#include "ym/ym_sat.h"


BEGIN_NAMESPACE_YM_SATPG

class TpgMap;

//////////////////////////////////////////////////////////////////////
/// @class TpgNode TpgNode.h "TpgNode.h"
/// @brief SATPG 用のノードを表すクラス
///
/// 基本的には一つの BnNode に対応しているが，
/// もとのゲートが組み込み型でない場合には複数の TpgNode を組み合わ
/// せてもとのゲートを表す．
/// そのため，場合によってはファンインの故障を表すための仮想的な
/// ノードを挿入する場合もある．
//////////////////////////////////////////////////////////////////////
class TpgNode
{
public:

  /// @brief 入力ノードを生成する．
  /// @param[in] alloc メモリアロケータ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] input_id 入力番号
  static
  TpgNode*
  new_input(Alloc& alloc,
	    ymuint id,
	    const char* name,
	    ymuint input_id);

  /// @brief 出力ノードを生成する．
  /// @param[in] alloc メモリアロケータ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] output_id 入力番号
  /// @param[in] inode ファンインのノード
  static
  TpgNode*
  new_output(Alloc& alloc,
	     ymuint id,
	     const char* name,
	     ymuint output_id,
	     TpgNode* inode);

  /// @brief 組み込み型の論理ゲートを生成する．
  /// @param[in] alloc メモリアロケータ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] type ゲートの型
  /// @param[in] inode_list ファンインのリスト
  /// @return 生成したノードを返す．
  static
  TpgNode*
  new_primitive(Alloc& alloc,
		ymuint id,
		const char* name,
		GateType type,
		const vector<TpgNode*>& inode_list);

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  TpgNode(ymuint id,
	  const char* name);

  /// @brief デストラクタ
  virtual
  ~TpgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  ///
  /// nullptr が返される場合もある．
  const char*
  name() const;

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  virtual
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ///
  /// node = TpgNetwork::input(node->input_id()
  /// の関係を満たす．
  /// is_input() が false の場合の返り値は不定
  virtual
  ymuint
  input_id() const;

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

  /// @brief logic タイプの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  virtual
  GateType
  gate_type() const;

  /// @brief controling value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  nval() const;

  /// @brief controling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  /// ない場合は kValX を返す．
  virtual
  Val3
  noval() const;

  /// @brief もとのゲートのファンインに対応するノードを返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  const TpgNode*
  input_map(ymuint pos) const;

  /// @brief もとのゲートのファンインに対応するノードを返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  TpgNode*
  input_map(ymuint pos);

  /// @brief BnNode のファンインに対応するノードのファンイン番号を返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  ymuint
  ipos_map(ymuint pos) const;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
  TpgNode*
  fanout(ymuint pos) const;

  /// @brief アクティブなファンアウト数を得る．
  ymuint
  active_fanout_num() const;

  /// @brief アクティブなファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
  TpgNode*
  active_fanout(ymuint pos) const;

  /// @brief アクティブの場合 true を返す．
  bool
  is_active() const;

  /// @brief FFR の根のノードを得る．
  ///
  /// 自分が根の場合には自分自身を返す．
  TpgNode*
  ffr_root() const;

  /// @brief 直近の dominator を得る．
  TpgNode*
  imm_dom() const;

  /// @brief MFFC 内の FFR の根のノードの数を返す．
  ///
  /// imm_dom() == nullptr の時のみ意味を持つ．
  ymuint
  mffc_elem_num() const;

  /// @brief MFFC 内の FFR の根のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < root_num() )
  ///
  /// imm_dom() == nullptr の時のみ意味を持つ．
  TpgNode*
  mffc_elem(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // CNF の生成に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の関係を表す CNF 式を生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lit_map 入出力とリテラルの対応マップ
  virtual
  void
  make_cnf(SatSolver& solver,
	   const LitMap& lit_map) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  TpgFault*
  output_fault(int val);

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  TpgFault*
  input_fault(int val,
	      ymuint pos);

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  virtual
  const TpgFault*
  output_fault(int val) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;

  /// @brief このノードに関係する代表故障数を返す．
  ymuint
  fault_num() const;

  /// @brief このノードに関係する代表故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  fault(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力番号2をセットする．
  /// @param[in] id セットする番号
  ///
  /// 出力ノード以外では無効
  virtual
  void
  set_output_id2(ymuint id);

  /// @brief ファンアウト用の配列を初期化する．
  /// @param[in] nfo ファンアウト数
  /// @param[in] fo_array ファンアウト用の配列
  /// @param[in] act_fo_array アクティブなファンアウト用の配列
  void
  set_fanout_num(ymuint nfo,
		 TpgNode** fo_array,
		 TpgNode** act_fo_array);

  /// @brief ファンアウトを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
  /// @param[in] fo_node ファンアウト先のノード
  void
  set_fanout(ymuint pos,
	     TpgNode* fo_node);

  /// @brief アクティブなファンアウト配列を設定する．
  /// @param[in] act_fanouts ファンアウト配列のソース
  void
  set_active_fanouts(const vector<TpgNode*>& act_fanouts);

  /// @brief TpgMap をセットする．
  /// @param[in] tmap セットする TpgMap
  void
  set_tmap(TpgMap* tmap);

  /// @brief FFR の根のノードをセットする．
  /// @param[in] root 根のノード
  void
  set_ffr_root(TpgNode* root);

  /// @brief immediate dominator をセットする．
  /// @param[in] dom dominator ノード
  void
  set_imm_dom(TpgNode* dom);

  /// @brief 出力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] fault 故障
  virtual
  void
  set_output_fault(int val,
		   TpgFault* fault);

  /// @brief 入力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  /// @param[in] fault 故障
  virtual
  void
  set_input_fault(int val,
		  ymuint pos,
		  TpgFault* fault);

  /// @brief 代表故障のリストをセットする．
  void
  set_fault_list(ymuint nf,
		 const TpgFault** fault_list);

  /// @brief アクティブにする．
  void
  set_active();

  /// @brief アクティブフラグを消す．
  void
  clear_active();

  /// @brief MFFC の情報をセットする．
  void
  set_root_list(ymuint n,
		TpgNode** root_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint mId;

  // 名前
  const char* mName;

  // もとのゲートの入力の対応関係
  // nullptr の時は内部ノード
  const TpgMap* mMap;

  // ファンアウト数
  ymuint mFanoutNum;

  // ファンアウトの配列
  TpgNode** mFanouts;

  // アクティブなファンアウト数
  ymuint mActFanoutNum;

  // アクティブなファンアウトの配列
  TpgNode** mActFanouts;

  // 代表故障数
  ymuint mFaultNum;

  // 代表故障の配列
  const TpgFault** mFaultList;

  // いくつかのマークを納めるビットベクタ
  ymuint32 mMarks;

  // FFR の根のノード
  TpgNode* mFfrRoot;

  // immediate dominator
  TpgNode* mImmDom;

  // MFFC 内の FFR の根のノードの数
  ymuint mRootNum;

  // MFFC 内の FFR の根のノードの配列
  TpgNode** mRootList;

};

/// @relates TpgNode
/// @brief ノード名を出力する
/// @param[in] s 出力先のストリーム
/// @param[in] node 対象のノード
void
print_node(ostream& s,
	   const TpgNode* node);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファンアウト数を得る．
inline
ymuint
TpgNode::fanout_num() const
{
  return mFanoutNum;
}

// @brief ファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
inline
TpgNode*
TpgNode::fanout(ymuint pos) const
{
  ASSERT_COND( pos < fanout_num() );
  return mFanouts[pos];
}

// @brief アクティブなファンアウト数を得る．
inline
ymuint
TpgNode::active_fanout_num() const
{
  return mActFanoutNum;
}

// @brief アクティブなファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
inline
TpgNode*
TpgNode::active_fanout(ymuint pos) const
{
  ASSERT_COND( pos < mActFanoutNum );
  return mActFanouts[pos];
}

// @brief このノードに関係する代表故障数を返す．
inline
ymuint
TpgNode::fault_num() const
{
  return mFaultNum;
}

// @brief このノードに関係する代表故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
inline
const TpgFault*
TpgNode::fault(ymuint pos) const
{
  ASSERT_COND( pos < mFaultNum );
  return mFaultList[pos % mFaultNum];
}

// @brief アクティブの場合 true を返す．
inline
bool
TpgNode::is_active() const
{
  return static_cast<bool>(mMarks & 1U);
}

// @brief アクティブにする．
inline
void
TpgNode::set_active()
{
  mMarks |= 1U;
}

// @brief アクティブフラグを消す．
inline
void
TpgNode::clear_active()
{
  mMarks &= ~1U;
}

// @brief FFR の根のノードを得る．
//
// 自分が根の場合には自分自身を返す．
inline
TpgNode*
TpgNode::ffr_root() const
{
  return mFfrRoot;
}

// @brief 直近の dominator を得る．
inline
TpgNode*
TpgNode::imm_dom() const
{
  return mImmDom;
}

// @brief MFFC 内の FFR の根のノードの数を返す．
//
// imm_dom() == nullptr の時のみ意味を持つ．
inline
ymuint
TpgNode::mffc_elem_num() const
{
  return mRootNum;
}

// @brief MFFC 内の FFR の根のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < root_num() )
//
// imm_dom() == nullptr の時のみ意味を持つ．
inline
TpgNode*
TpgNode::mffc_elem(ymuint pos) const
{
  ASSERT_COND( pos < mffc_elem_num() );
  return mRootList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // TPGNODE_H

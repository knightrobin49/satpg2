
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "TpgInput.h"
#include "TpgOutput.h"
#include "TpgLogicC0.h"
#include "TpgLogicC1.h"
#include "TpgLogicBUFF.h"
#include "TpgLogicNOT.h"
#include "TpgLogicAND.h"
#include "TpgLogicNAND.h"
#include "TpgLogicOR.h"
#include "TpgLogicNOR.h"
#include "TpgLogicXOR.h"
#include "TpgLogicXNOR.h"
#include "TpgMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// 指定された型の配列を確保するテンプレート関数
template <typename T>
T*
alloc_array(Alloc& alloc,
	    ymuint n)
{
  void* p = alloc.get_memory(sizeof(T) * n);
  return new (p) T[n];
}

END_NONAMESPACE

// @brief GateType のストリーム演算子
ostream&
operator<<(ostream& s,
	   GateType gate_type)
{
  switch ( gate_type ) {
  case kGateCONST0: s << "CONST-0"; break;
  case kGateCONST1: s << "CONST-1"; break;
  case kGateBUFF:   s << "BUFF"; break;
  case kGateNOT:    s << "NOT"; break;
  case kGateAND:    s << "AND"; break;
  case kGateNAND:   s << "NAND"; break;
  case kGateOR:     s << "OR"; break;
  case kGateNOR:    s << "NOR"; break;
  case kGateXOR:    s << "XOR"; break;
  case kGateXNOR:   s << "XNOR"; break;
  default:          s << "---"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgNode
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードを生成する．
// @param[in] alloc メモリアロケータ
// @param[in] id ID番号
// @param[in] name ノード名
// @param[in] input_id 入力番号
TpgNode*
TpgNode::new_input(Alloc& alloc,
		   ymuint id,
		   const char* name,
		   ymuint input_id)
{
  void* p = alloc.get_memory(sizeof(TpgInput));
  TpgNode* node = new (p) TpgInput(id, name, input_id);

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] alloc メモリアロケータ
// @param[in] id ID番号
// @param[in] name ノード名
// @param[in] output_id 入力番号
// @param[in] inode ファンインのノード
TpgNode*
TpgNode::new_output(Alloc& alloc,
		    ymuint id,
		    const char* name,
		    ymuint output_id,
		    TpgNode* inode)
{
  void* p = alloc.get_memory(sizeof(TpgOutput));
  TpgNode* node = new (p) TpgOutput(id, name, output_id, inode);

  return node;
}

// @brief 組み込み型の論理ゲートを生成する．
// @param[in] alloc メモリアロケータ
// @param[in] id ID番号
// @param[in] name ノード名
// @param[in] type ゲートの型
// @param[in] inode_list ファンインのリスト
// @return 生成したノードを返す．
TpgNode*
TpgNode::new_primitive(Alloc& alloc,
		       ymuint id,
		       const char* name,
		       GateType type,
		       const vector<TpgNode*>& inode_list)
{
  TpgNode* node = nullptr;

  ymuint ni = inode_list.size();

  void* p;
  switch ( type ) {
  case kGateCONST0 :
    p = alloc.get_memory(sizeof(TpgLogicC0));
    node = new (p) TpgLogicC0(id, name);
    break;

  case kGateCONST1:
    p = alloc.get_memory(sizeof(TpgLogicC1));
    node = new (p) TpgLogicC1(id, name);
    break;

  case kGateBUFF:
    p = alloc.get_memory(sizeof(TpgLogicBUFF));
    node = new (p) TpgLogicBUFF(id, name, inode_list[0]);
    break;

  case kGateNOT:
    p = alloc.get_memory(sizeof(TpgLogicNOT));
    node = new (p) TpgLogicNOT(id, name, inode_list[0]);
    break;

  case kGateAND:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicAND2));
      node = new (p) TpgLogicAND2(id, name,
				  inode_list[0], inode_list[1]);
      break;

    case 3:
      p = alloc.get_memory(sizeof(TpgLogicAND3));
      node = new (p) TpgLogicAND3(id, name,
				  inode_list[0], inode_list[1],
				  inode_list[2]);
      break;

    case 4:
      p = alloc.get_memory(sizeof(TpgLogicAND4));
      node = new (p) TpgLogicAND4(id, name,
				  inode_list[0], inode_list[1],
				  inode_list[2], inode_list[3]);
      break;

    default:
      {
	TpgNode** fanin_array = alloc_array<TpgNode*>(alloc, ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  fanin_array[i] = inode_list[i];
	}
	TpgFault** fault_array = alloc_array<TpgFault*>(alloc, ni * 2);
	p = alloc.get_memory(sizeof(TpgLogicANDN));
	node = new (p) TpgLogicANDN(id, name, ni, fanin_array, fault_array);
      }
      break;
    }
    break;

  case kGateNAND:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicNAND2));
      node = new (p) TpgLogicNAND2(id, name,
				   inode_list[0], inode_list[1]);
      break;

    case 3:
      p = alloc.get_memory(sizeof(TpgLogicNAND3));
      node = new (p) TpgLogicNAND3(id, name,
				   inode_list[0], inode_list[1],
				   inode_list[2]);
      break;

    case 4:
      p = alloc.get_memory(sizeof(TpgLogicNAND4));
      node = new (p) TpgLogicNAND4(id, name,
				   inode_list[0], inode_list[1],
				   inode_list[2], inode_list[3]);
      break;

    default:
      {
	TpgNode** fanin_array = alloc_array<TpgNode*>(alloc, ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  fanin_array[i] = inode_list[i];
	}
	TpgFault** fault_array = alloc_array<TpgFault*>(alloc, ni * 2);
	p = alloc.get_memory(sizeof(TpgLogicNANDN));
	node = new (p) TpgLogicNANDN(id, name, ni, fanin_array, fault_array);
      }
      break;
    }
    break;

  case kGateOR:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicOR2));
      node = new (p) TpgLogicOR2(id, name,
				 inode_list[0], inode_list[1]);
      break;

    case 3:
      p = alloc.get_memory(sizeof(TpgLogicOR3));
      node = new (p) TpgLogicOR3(id, name,
				 inode_list[0], inode_list[1],
				 inode_list[2]);
      break;

    case 4:
      p = alloc.get_memory(sizeof(TpgLogicOR4));
      node = new (p) TpgLogicOR4(id, name,
				 inode_list[0], inode_list[1],
				 inode_list[2], inode_list[3]);
      break;

    default:
      {
	TpgNode** fanin_array = alloc_array<TpgNode*>(alloc, ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  fanin_array[i] = inode_list[i];
	}
	TpgFault** fault_array = alloc_array<TpgFault*>(alloc, ni * 2);
	p = alloc.get_memory(sizeof(TpgLogicORN));
	node = new (p) TpgLogicORN(id, name, ni, fanin_array, fault_array);
      }
      break;
    }
    break;

  case kGateNOR:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicNOR2));
      node = new (p) TpgLogicNOR2(id, name,
				  inode_list[0], inode_list[1]);
      break;

    case 3:
      p = alloc.get_memory(sizeof(TpgLogicNOR3));
      node = new (p) TpgLogicNOR3(id, name,
				  inode_list[0], inode_list[1],
				  inode_list[2]);
      break;

    case 4:
      p = alloc.get_memory(sizeof(TpgLogicNOR4));
      node = new (p) TpgLogicNOR4(id, name,
				  inode_list[0], inode_list[1],
				  inode_list[2], inode_list[3]);
      break;

    default:
      {
	TpgNode** fanin_array = alloc_array<TpgNode*>(alloc, ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  fanin_array[i] = inode_list[i];
	}
	TpgFault** fault_array = alloc_array<TpgFault*>(alloc, ni * 2);
	p = alloc.get_memory(sizeof(TpgLogicNORN));
	node = new (p) TpgLogicNORN(id, name, ni, fanin_array, fault_array);
      }
      break;
    }
    break;

  case kGateXOR:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicXOR2));
      node = new (p) TpgLogicXOR2(id, name,
				  inode_list[0], inode_list[1]);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case kGateXNOR:
    switch ( ni ) {
    case 2:
      p = alloc.get_memory(sizeof(TpgLogicXNOR2));
      node = new TpgLogicXNOR2(id, name,
			       inode_list[0], inode_list[1]);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  return node;
}

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
TpgNode::TpgNode(ymuint id,
		 const char* name) :
  mId(id),
  mName(name)
{
  mMap = nullptr;
  mFanoutNum = 0;
  mFanouts = nullptr;
  mActFanoutNum = 0;
  mActFanouts = nullptr;
  mFaultNum = 0;
  mFaultList = nullptr;
  mMarks = 0U;
  mRootNum = 0;
  mRootList = nullptr;
}

// @brief デストラクタ
TpgNode::~TpgNode()
{
}

// @brief ID番号を得る．
ymuint
TpgNode::id() const
{
  return mId;
}

// @brief 名前を得る．
const char*
TpgNode::name() const
{
  return mName;
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
bool
TpgNode::is_input() const
{
  return false;
}

// @brief 外部入力タイプの時に入力番号を返す．
ymuint
TpgNode::input_id() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
bool
TpgNode::is_output() const
{
  return false;
}

// @brief 外部出力タイプの時に出力番号を返す．
ymuint
TpgNode::output_id() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
ymuint
TpgNode::output_id2() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief logic タイプの時 true を返す．
bool
TpgNode::is_logic() const
{
  return false;
}

// @brief ゲートタイプを得る．
GateType
TpgNode::gate_type() const
{
  ASSERT_NOT_REACHED;
  return kGateCONST0;
}

// @brief controling value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::coval() const
{
  return kValX;;
}

// @brief noncontroling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::noval() const
{
  return kValX;
}

// @brief ファンイン数を得る．
ymuint
TpgNode::fanin_num() const
{
  return 0;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgNode::fanin(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief TgNode のファンインに対応するノードを返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
const TpgNode*
TpgNode::input_map(ymuint pos) const
{
  if ( mMap == nullptr ) {
    return this;
  }
  else {
    return mMap->input(pos);
  }
}

// @brief TgNode のファンインに対応するノードを返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
TpgNode*
TpgNode::input_map(ymuint pos)
{
  if ( mMap == nullptr ) {
    return this;
  }
  else {
    return mMap->input(pos);
  }
}

// @brief TgNode のファンインに対応するノードのファンイン番号を返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
ymuint
TpgNode::ipos_map(ymuint pos) const
{
  if ( mMap == nullptr ) {
    return pos;
  }
  else {
    return mMap->input_pos(pos);
  }
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgNode::make_cnf(SatSolver& solver,
		  const LitMap& lit_map) const
{
  ASSERT_NOT_REACHED;
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgNode::make_faulty_cnf(SatSolver& solver,
			 ymuint fpos,
			 int fval,
			 const LitMap& lit_map) const
{
  ASSERT_NOT_REACHED;
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgNode::output_fault(int val) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgNode::input_fault(int val,
		     ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
TpgFault*
TpgNode::output_fault(int val)
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgNode::input_fault(int val,
		     ymuint pos)
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 出力番号2をセットする．
// @param[in] id セットする番号
//
// 出力ノード以外では無効
void
TpgNode::set_output_id2(ymuint id)
{
  ASSERT_NOT_REACHED;
}

// @brief ファンアウト用の配列を初期化する．
// @param[in] nfo ファンアウト数
// @param[in] fo_array ファンアウト用の配列
// @param[in] act_fo_array アクティブなファンアウト用の配列
void
TpgNode::set_fanout_num(ymuint nfo,
			TpgNode** fo_array,
			TpgNode** act_fo_array)
{
  mFanoutNum = nfo;
  mFanouts = fo_array;
  mActFanoutNum = 0;
  mActFanouts = act_fo_array;
}

// @brief ファンアウトを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
// @param[in] fo_node ファンアウト先のノード
void
TpgNode::set_fanout(ymuint pos,
		    TpgNode* fo_node)
{
  mFanouts[pos] = fo_node;
}

// @brief アクティブなファンアウト配列を設定する．
// @param[in] act_fanouts ファンアウト配列のソース
void
TpgNode::set_active_fanouts(const vector<TpgNode*>& act_fanouts)
{
  ymuint nf = act_fanouts.size();
  mActFanoutNum = nf;
  for (ymuint i = 0; i < nf; ++ i) {
    mActFanouts[i] = act_fanouts[i];
  }
}

// @brief TpgMap をセットする．
// @param[in] tmap セットする TpgMap
void
TpgNode::set_tmap(TpgMap* tmap)
{
  mMap = tmap;
}

// @brief FFR の根のノードをセットする．
// @param[in] root 根のノード
void
TpgNode::set_ffr_root(TpgNode* root)
{
  mFfrRoot = root;
}

// @brief immediate dominator をセットする．
// @param[in] dom dominator ノード
void
TpgNode::set_imm_dom(TpgNode* dom)
{
  mImmDom = dom;
}

// @brief 入力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
// @param[in] fault 故障
void
TpgNode::set_input_fault(int val,
			 ymuint pos,
			 TpgFault* fault)
{
  ASSERT_NOT_REACHED;
}

// @brief 出力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] fault 故障
void
TpgNode::set_output_fault(int val,
			  TpgFault* fault)
{
  ASSERT_NOT_REACHED;
}

// @brief 代表故障のリストをセットする．
void
TpgNode::set_fault_list(ymuint nf,
			const TpgFault** fault_list)
{
  mFaultNum = nf;
  mFaultList = fault_list;
}

// @brief MFFC の情報をセットする．
void
TpgNode::set_root_list(ymuint n,
		       TpgNode** root_list)
{
  mRootNum = n;
  mRootList = root_list;
}

END_NAMESPACE_YM_SATPG

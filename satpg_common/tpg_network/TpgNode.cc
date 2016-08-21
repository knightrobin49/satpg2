
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "ym/SatSolver.h"

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


BEGIN_NAMESPACE_YM_SATPG

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

// @brief 入力ノードを作る．
// @param[in] id ノード番号
// @param[in] iid 入力番号
// @param[in] fanout_num ファンアウト数
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_input(ymuint id,
		    ymuint iid,
		    ymuint fanout_num)
{
  TpgNode* node = new TpgNode(id, vector<TpgNode*>(), fanout_num);
  node->mTypeId = (1U | (iid << 2));

  return node;
}

// @brief 出力ノードを作る．
// @param[in] id ノード番号
// @param[in] oid 出力番号
// @param[in] inode 入力ノード
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_output(ymuint id,
		     ymuint oid,
		     TpgNode* inode)
{
  TpgNode* node = new TpgNode(id, vector<TpgNode*>(1, inode), 0);
  node->mTypeId = (2U | (oid << 2));

  return node;
}

// @brief 論理ノードを作る．
// @param[in] id ノード番号
// @param[in] gate_type ゲートタイプ
// @param[in] inode_list 入力ノードのリスト
// @param[in] fanout_num ファンアウト数
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_logic(ymuint id,
		    GateType gate_type,
		    const vector<TpgNode*>& inode_list,
		    ymuint fanout_num)
{
  ymuint ni = inode_list.size();
  TpgNode* node = nullptr;
  switch ( gate_type ) {
  case kGateCONST0:
    node = new TpgLogicC0(id, fanout_num);
    break;

  case kGateCONST1:
    node = new TpgLogicC1(id, fanout_num);
    break;

  case kGateBUFF:
    node = new TpgLogicBUFF(id, inode_list, fanout_num);
    break;

  case kGateNOT:
    node = new TpgLogicNOT(id, inode_list, fanout_num);
    break;

  case kGateAND:
    switch ( ni ) {
    case 2:
      node = new TpgLogicAND2(id, inode_list, fanout_num);
      break;

    case 3:
      node = new TpgLogicAND3(id, inode_list, fanout_num);
      break;

    case 4:
      node = new TpgLogicAND4(id, inode_list, fanout_num);
      break;

    default:
      node = new TpgLogicAND(id, inode_list, fanout_num);
      break;
    }
    break;

  case kGateNAND:
    switch ( ni ) {
    case 2:
      node = new TpgLogicNAND2(id, inode_list, fanout_num);
      break;

    case 3:
      node = new TpgLogicNAND3(id, inode_list, fanout_num);
      break;

    case 4:
      node = new TpgLogicNAND4(id, inode_list, fanout_num);
      break;

    default:
      node = new TpgLogicNAND(id, inode_list, fanout_num);
      break;
    }
    break;

  case kGateOR:
    switch ( ni ) {
    case 2:
      node = new TpgLogicOR2(id, inode_list, fanout_num);
      break;

    case 3:
      node = new TpgLogicOR3(id, inode_list, fanout_num);
      break;

    case 4:
      node = new TpgLogicOR4(id, inode_list, fanout_num);
      break;

    default:
      node = new TpgLogicOR(id, inode_list, fanout_num);
      break;
    }
    break;

  case kGateNOR:
    switch ( ni ) {
    case 2:
      node = new TpgLogicNOR2(id, inode_list, fanout_num);
      break;

    case 3:
      node = new TpgLogicNOR3(id, inode_list, fanout_num);
      break;

    case 4:
      node = new TpgLogicNOR4(id, inode_list, fanout_num);
      break;

    default:
      node = new TpgLogicNOR(id, inode_list, fanout_num);
      break;
    }
    break;

  case kGateXOR:
    ASSERT_COND( ni == 2 );
    node = new TpgLogicXOR2(id, inode_list, fanout_num);
    break;

  case kGateXNOR:
    ASSERT_COND( ni == 2 );
    node = new TpgLogicXNOR2(id, inode_list, fanout_num);
    break;

  default:
    ASSERT_NOT_REACHED;
  }

  node->mTypeId = (3U | (static_cast<ymuint32>(gate_type) << 2));

  return node;
}

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
TpgNode::TpgNode(ymuint id,
		 const vector<TpgNode*>& fanin_list,
		 ymuint fanout_num) :
  mId(id),
  mFaninNum(fanin_list.size()),
  mFanoutNum(fanout_num)
{
  mFaninList = new TpgNode*[mFaninNum];
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    mFaninList[i] = fanin_list[i];
  }

  mFanoutList = new TpgNode*[mFanoutNum];
}

// @brief デストラクタ
TpgNode::~TpgNode()
{
  delete [] mFaninList;
  delete [] mFanoutList;
}

// @brief ID番号を得る．
ymuint
TpgNode::id() const
{
  return mId;
}

// @brief controling value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::cval() const
{
  ASSERT_COND( is_output() );
  return kValX;
}

// @brief noncontroling valueを得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::nval() const
{
  ASSERT_COND( is_output() );
  return kValX;
}

// @brief controling output value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::coval() const
{
  ASSERT_COND( is_output() );
  return kValX;;
}

// @brief noncontroling output value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::noval() const
{
  ASSERT_COND( is_output() );
  return kValX;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgNode::make_cnf(SatSolver& solver,
		  const LitMap& lit_map) const
{
  if ( is_input() ) {
    //cout << "INPUT" << endl;
    // なにもしない．
    return;
  }

  SatLiteral olit = lit_map.output();
  if ( is_output() ) {
    //cout << "OUTPUT" << endl;
    SatLiteral ilit = lit_map.input(0);
    solver.add_clause( ilit, ~olit);
    solver.add_clause(~ilit,  olit);

    return;
  }

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

// @brief 出力番号2をセットする．
// @param[in] id セットする番号
//
// 出力ノード以外では無効
void
TpgNode::set_output_id2(ymuint id)
{
  ASSERT_COND( is_output() );
  mOutputId2 = id;
}

// @brief ファンアウトを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
// @param[in] fo_node ファンアウト先のノード
void
TpgNode::set_fanout(ymuint pos,
		    TpgNode* fo_node)
{
  ASSERT_COND( pos < fanout_num() );
  mFanoutList[pos] = fo_node;
}

// @brief immediate dominator をセットする．
// @param[in] dom dominator ノード
void
TpgNode::set_imm_dom(const TpgNode* dom)
{
  mImmDom = dom;
}

// @brief MFFC 内の根のノードの情報をセットする．
// @param[in] num 要素数
// @param[in] node_list ノードのリスト
void
TpgNode::set_mffc_info(ymuint num,
		       TpgNode** node_list)
{
  mMffcElemNum = num;
  mMffcElemList = node_list;
}

END_NAMESPACE_YM_SATPG

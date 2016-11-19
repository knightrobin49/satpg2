#ifndef BT2JUST2_H
#define BT2JUST2_H

/// @file Bt2Just2.h
/// @brief Bt2Just2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2JustBase.h"
#include "ym/UnitAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Bt2Just2 Bt2Just2.h "Bt2Just2.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class Bt2Just2 :
  public Bt2JustBase
{
public:

  /// @brief コンストラクタ
  Bt2Just2();

  /// @brief デストラクタ
  virtual
  ~Bt2Just2();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id);

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] output_list 故障に関係する出力ノードのリスト
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const vector<const TpgNode*>& output_list,
      const ValMap2& val_map,
      NodeVal2List& assign_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // Bt2JustBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief clear_justified() 中で呼ばれるフック関数
  virtual
  void
  clear_justified_hook(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeList
  {
    const TpgNode* mNode;

    int mTime;

    NodeList* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  NodeList*
  justify(const TpgNode* node,
	  const ValMap2& val_map);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  NodeList*
  just_sub1(const TpgNode* node,
	    const ValMap2& val_map);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  NodeList*
  just_sub2(const TpgNode* node,
	    const ValMap2& val_map,
	    Val3 val);

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  NodeList*
  justify0(const TpgNode* node,
	   const ValMap2& val_map);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  NodeList*
  just0_sub1(const TpgNode* node,
	     const ValMap2& val_map);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  NodeList*
  just0_sub2(const TpgNode* node,
	     const ValMap2& val_map,
	     Val3 val);

  /// @brief 新しいリストのセルを返す．
  NodeList*
  new_list_cell(const TpgNode* node,
		int time);

  /// @brief リストをマージする．
  void
  list_merge(NodeList*& dst_list,
	     NodeList* src_list);

  /// @brief リストのサイズを返す．
  static
  ymuint
  list_size(NodeList* node_list);

  /// @brief リストを削除する．
  void
  list_free(NodeList* node_list);

  /// @brief ノードの TFO に印をつける．
  void
  mark_tfo(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  UnitAlloc mAlloc;

  // ノード番号の最大値
  ymuint mMaxId;

  // node->id() をキーにして入力番号のリストを納める配列
  vector<NodeList*> mJustArray;

  // node->id() をキーにして入力番号のリストを納める配列
  vector<NodeList*> mJust0Array;

  // 故障の TFO につけるマーク
  vector<bool> mTfoMark;

};

END_NAMESPACE_YM_SATPG

#endif // BT2JUST2_H

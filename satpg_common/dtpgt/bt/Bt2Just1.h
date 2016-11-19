#ifndef BT2JUST1_H
#define BT2JUST1_H

/// @file Bt2Just1.h
/// @brief Bt2Just1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2JustBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Bt2Just1 Bt2Just1.h "Bt2Just1.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class Bt2Just1 :
  public Bt2JustBase
{
public:

  /// @brief コンストラクタ
  Bt2Just1();

  /// @brief デストラクタ
  virtual
  ~Bt2Just1();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

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
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(const TpgNode* node,
	  const ValMap2& val_map,
	  NodeVal2List& assign_list);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  void
  just_sub1(const TpgNode* node,
	    const ValMap2& val_map,
	    NodeVal2List& assign_list);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  /// @param[out] assign_list 値の割当リスト
  void
  just_sub2(const TpgNode* node,
	    const ValMap2& val_map,
	    Val3 val,
	    NodeVal2List& assign_list);

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify0(const TpgNode* node,
	   const ValMap2& val_map,
	   NodeVal2List& assign_list);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  void
  just0_sub1(const TpgNode* node,
	     const ValMap2& val_map,
	     NodeVal2List& assign_list);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  /// @param[out] assign_list 値の割当リスト
  void
  just0_sub2(const TpgNode* node,
	     const ValMap2& val_map,
	     Val3 val,
	     NodeVal2List& assign_list);

};

END_NAMESPACE_YM_SATPG

#endif // BT2JUST1_H

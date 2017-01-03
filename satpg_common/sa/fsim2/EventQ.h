#ifndef FSIM_EVENTQ_H
#define FSIM_EVENTQ_H

/// @file EventQ.h
/// @brief EventQ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class EventQ EventQ.h "EventQ.h"
/// @brief 故障シミュレーション用のイベントキュー
///
/// キューに詰まれる要素は SimNode で，各々のノードはレベルを持つ．
/// このキューではレベルの小さい順に処理してゆく．同じレベルのノード
/// 間の順序は任意でよい．
//////////////////////////////////////////////////////////////////////
class EventQ
{
public:

  /// @brief コンストラクタ
  EventQ();

  /// @brief デストラクタ
  ~EventQ();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] max_level 最大レベル
  /// @param[in] node_num ノード数
  void
  init(ymuint max_level,
       ymuint node_num);

  /// @brief 初期イベントを追加する．
  /// @param[in] node 対象のノード
  /// @param[in] valmask 反転マスク
  void
  put_trigger(SimNode* node,
	      PackedVal valmask);

  /// @brief イベントドリブンシミュレーションを行う．
  /// @param[in] target 目標のノード
  /// @retval 出力における変化ビットを返す．
  ///
  /// target が nullptr でない時にはイベントが target まで到達したら
  /// シミュレーションを終える．
  /// target が nullptr の時には出力ノードまでイベントを伝える．
  PackedVal
  simulate(SimNode* target = nullptr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンアウトのノードをキューに積む．
  /// @param[in] node 対象のノード
  void
  put_fanouts(SimNode* node);

  /// @brief キューに積む
  /// @param[in] node 対象のノード
  void
  put(SimNode* node);

  /// @brief キューから取り出す．
  /// @retval nullptr キューが空だった．
  SimNode*
  get();

  /// @brief clear リストに追加する．
  /// @param[in] node 対象のノード
  void
  add_to_clear_list(SimNode* node);

  /// @brief clear リストに入っているノードの値を元に戻す．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mArray のサイズ
  ymuint mArraySize;

  // キューの先頭ノードの配列
  SimNode** mArray;

  // 現在のレベル．
  ymuint mCurLevel;

  // キューに入っているノード数
  ymuint mNum;

  // mCearArray のサイズ
  ymuint mClearArraySize;

  // clear 用のノード配列
  SimNode** mClearArray;

  // mCelarArray の最後の要素位置
  ymuint mClearPos;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief キューに積む
inline
void
EventQ::put(SimNode* node)
{
  if ( !node->in_queue() ) {
    node->set_queue();
    ymuint level = node->level();
    SimNode*& w = mArray[level];
    node->mLink = w;
    w = node;
    if ( mNum == 0 || mCurLevel > level ) {
      mCurLevel = level;
    }
    ++ mNum;
  }
}

// @brief キューから取り出す．
// @retval nullptr キューが空だった．
inline
SimNode*
EventQ::get()
{
  if ( mNum > 0 ) {
    // mNum が正しければ mCurLevel がオーバーフローすることはない．
    for ( ; ; ++ mCurLevel) {
      SimNode*& w = mArray[mCurLevel];
      SimNode* node = w;
      if ( node ) {
	node->clear_queue();
	w = node->mLink;
	-- mNum;
	return node;
      }
    }
  }
  return nullptr;
}

// @brief clear 用リストに追加する．
inline
void
EventQ::add_to_clear_list(SimNode* node)
{
  mClearArray[mClearPos] = node;
  ++ mClearPos;
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_EVENTQ_H

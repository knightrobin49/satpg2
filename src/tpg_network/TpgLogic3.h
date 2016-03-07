#ifndef TPGLOGIC3_H
#define TPGLOGIC3_H

/// @file TpgLogic3.h
/// @brief TpgLogic3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogic3 TpgLogic3.h "TpgLogic3.h"
/// @brief 0入力の TpgLogic
//////////////////////////////////////////////////////////////////////
class TpgLogic3 :
  public TpgLogic
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] inode0, inode1, inode2 ファンインのノード
  TpgLogic3(ymuint id,
	    TpgNode* inode0,
	    TpgNode* inode1,
	    TpgNode* inode2);

  /// @brief デストラクタ
  ~TpgLogic3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  /// @param[in] fault 故障
  virtual
  void
  set_input_fault(int val,
		  ymuint pos,
		  TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンイン
  TpgNode* mFanins[3];

  // 入力の故障
  TpgFault* mInputFaults[6];

};

END_NAMESPACE_YM_SATPG

#endif // TPGLOGIC3_H

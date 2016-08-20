#ifndef AUXNODEINFO_H
#define AUXNODEINFO_H

/// @file AuxNodeInfo.h
/// @brief AuxNodeInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "ym/Alloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class AuxNodeInfo AuxNodeInfo.h "AuxNodeInfo.h"
/// @brief TpgNode の付加的な情報を保持するクラス
//////////////////////////////////////////////////////////////////////
class AuxNodeInfo
{
public:

  /// @brief コンストラクタ
  AuxNodeInfo();

  /// @brief デストラクタ
  ~AuxNodeInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const;

  /// @brief 代表故障数を返す．
  ymuint
  fault_num() const;

  /// @brief 代表故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  fault(ymuint pos) const;

  /// @brief 出力の故障を返す．
  /// @param[in] val 故障値 ( 0 / 1 )
  TpgFault*
  output_fault(int val) const;

  /// @brief 入力の故障を返す．
  /// @param[in] pos 入力の位置番号
  /// @param[in] val 故障値 ( 0 / 1 )
  TpgFault*
  input_fault(ymuint pos,
	      int val) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  /// @param[in] name 名前
  /// @param[in] alloc メモリアロケータ
  void
  set_name(const string& name,
	   Alloc& alloc);

  /// @brief 出力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] f 故障
  void
  set_output_fault(int val,
		   TpgFault* f);

  /// @brief 入力数を設定する．
  /// @param[in] ni 入力数
  /// @param[in] alloc メモリアロケータ
  void
  set_fanin_num(ymuint ni,
		Alloc& alloc);

  /// @brief 入力の故障を設定する．
  /// @param[in] ipos 入力位置
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] f 故障
  void
  set_input_fault(ymuint ipos,
		  int val,
		  TpgFault* f);

  /// @brief 故障リストを設定する．
  /// @param[in] fault_list 故障リスト
  /// @param[in] alloc メモリアロケータ
  void
  set_fault_list(const vector<const TpgFault*>& fault_list,
		 Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード名
  char* mName;

  /// @brief 代表故障数
  ymuint mFaultNum;

  /// @brief 代表故障の配列
  const TpgFault** mFaultList;

  /// @brief 出力の故障
  TpgFault* mOutputFaults[2];

  /// @brief 入力数
  ymuint mFaninNum;

  /// @brief 入力の故障の配列
  ///
  /// サイズは mFaninNum * 2
  TpgFault** mInputFaults;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前を返す．
inline
const char*
AuxNodeInfo::name() const
{
  return mName;
}

// @brief 代表故障数を返す．
inline
ymuint
AuxNodeInfo::fault_num() const
{
  return mFaultNum;
}

// @brief 代表故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
inline
const TpgFault*
AuxNodeInfo::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaultList[pos];
}

// @brief 出力の故障を返す．
// @param[in] val 故障値 ( 0 / 1 )
inline
TpgFault*
AuxNodeInfo::output_fault(int val) const
{
  ASSERT_COND( val == 0 || val == 1 );
  return mOutputFaults[val];
}

// @brief 入力の故障を返す．
// @param[in] pos 入力の位置番号
// @param[in] val 故障値 ( 0 / 1 )
inline
TpgFault*
AuxNodeInfo::input_fault(ymuint pos,
			 int val) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos < mFaninNum );
  return mInputFaults[(pos * 2) + val];
}

END_NAMESPACE_YM_SATPG

#endif // AUXNODEINFO_H

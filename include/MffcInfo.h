#ifndef MFFCINFO_H
#define MFFCINFO_H

/// @file MffcInfo.h
/// @brief MffcInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MffcInfo MffcInfo.h "MffcInfo.h"
/// @brief MFFC の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class MffcInfo
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_list 含まれるノードのリスト
  /// @param[in] input_list 葉のノードのリスト
  ///
  /// node_list[0] が根のノードとなっている必要がある．
  MffcInfo(const vector<const TpgNode*>& node_list,
	   const vector<const TpgNode*>& input_list);

  /// @brief デストラクタ
  ~MffcInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを返す．
  const TpgNode*
  root() const;

  /// @brief 葉のノード数を返す．
  ymuint
  input_num() const;

  /// @brief 葉のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  const TpgNode*
  input(ymuint pos) const;

  /// @brief 含まれるノード数を返す．
  ymuint
  node_num() const;

  /// @brief 含まれるノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  const TpgNode*
  node(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 中に含まれるノードのリスト
  // 根は含むが葉は含まない．
  vector<const TpgNode*> mNodeList;

  // 葉のノードのリスト
  vector<const TpgNode*> mInputList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_list 含まれるノードのリスト
// @param[in] input_list 葉のノードのリスト
//
// node_list[0] が根のノードとなっている必要がある．
inline
MffcInfo::MffcInfo(const vector<const TpgNode*>& node_list,
		   const vector<const TpgNode*>& input_list) :
  mNodeList(node_list),
  mInputList(input_list)
{
}

// @brief デストラクタ
inline
MffcInfo::~MffcInfo()
{
}

// @brief 根のノードを返す．
inline
const TpgNode*
MffcInfo::root() const
{
  ASSERT_COND( node_num() > 0 );
  return mNodeList[0];
}

// @brief 葉のノード数を返す．
inline
ymuint
MffcInfo::input_num() const
{
  return mInputList.size();
}

// @brief 葉のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
inline
const TpgNode*
MffcInfo::input(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

// @brief 含まれるノード数を返す．
inline
ymuint
MffcInfo::node_num() const
{
  return mNodeList.size();
}

// @brief 含まれるノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
inline
const TpgNode*
MffcInfo::node(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // MFFCINFO_H

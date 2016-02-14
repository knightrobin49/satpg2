#ifndef TPGISCAS89READER_H
#define TPGISCAS89READER_H

/// @file TgIscas89Reader.h
/// @brief TgIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "ym/ym_iscas89.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgIscas89Reader TpgIscas89Reader.h "TpgIscas89Reader.h"
/// @brief iscas89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class TpgIscas89Reader
{
public:

  /// @brief コンストラクタ
  TpgIscas89Reader();

  /// @brief デストラクタ
  ~TpgIscas89Reader();


public:

  /// @brief blif ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] network 設定対象のネットワーク
  bool
  operator()(const string& filename,
	     TpgNetwork& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ISCAS89 ファイルを読み込むためのパーサ
  Iscas89Parser* mParser;

  // ISCAS89 ファイルを TpgNetwork に変換するためのハンドラ
  TpgIscas89Handler* mHandler;

};

END_NAMESPACE_YM_SATPG

#endif // TPGISCAS89READER_H

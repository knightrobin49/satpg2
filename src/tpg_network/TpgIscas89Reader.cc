
/// @file TpgIscas89Reader
/// @brief TpgIscas89Reaer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgIscas89Reader.h"
#include "ym/Iscas89Parser.h"
#include "TpgIscas89Handler.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// ISCAS89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TpgIscas89Reader::TpgIscas89Reader() :
  mParser(new Iscas89Parser),
  mHandler(new TpgIscas89Handler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
TpgIscas89Reader::~TpgIscas89Reader()
{
  delete mParser;
  // mHandler は Iscas89Parser が責任をもって破壊してくれる．
}

// @brief blif ファイルを読み込む．
bool
TpgIscas89Reader::operator()(const string& filename,
			     TpgNetwork& network)
{
  mHandler->set_network(network);
  bool stat = mParser->read(filename);
  return stat;
}

END_NAMESPACE_YM_SATPG

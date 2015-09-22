
/// @file pure-mc.cc
/// @brief pure-mc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNode.h"

#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM

ymuint
count(const BdnMgr& network,
      const vector<ymuint>& pat1,
      const vector<ymuint>& pat2);

void
randpat(RandGen& rg,
	ymuint ni,
	vector<ymuint>& pat)
{
  ymuint32 r = rg.int32();
  ymuint pos = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    if ( r & (1U << pos) ) {
      pat[i] = 1;
    }
    else {
      pat[i] = 0;
    }
    ++ pos;
    if ( pos == 32 ) {
      r = rg.int32();
      pos = 0;
    }
  }
}

ymuint
pure_mc(const BdnMgr& network,
	ymuint n_samples)
{
  RandGen rg;
  ymuint ni = network.input_num();
  ymuint sum_c = 0;
  ymuint max_c = 0;
  ymuint loop_count = n_samples;
  for (ymuint l = 0; l < loop_count; ++ l) {
    vector<ymuint> pat1(ni);
    vector<ymuint> pat2(ni);
    randpat(rg, ni, pat1);
    randpat(rg, ni, pat2);
    ymuint c = count(network, pat1, pat2);
    sum_c += c;
    if ( max_c < c ) {
      max_c = c;
    }
  }
  {
    double ave_c = static_cast<double>(sum_c) / static_cast<double>(loop_count);
    cout << "average switching:        " << ave_c << endl
	 << "max switching:            " << max_c << endl;
  }

  return max_c;
}

END_NAMESPACE_YM

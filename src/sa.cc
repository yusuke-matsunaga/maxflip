
/// @file sa.cc
/// @brief sa の実装ファイル
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

ymuint
count(const BdnMgr& network,
      const vector<ymuint>& state)
{
  ymuint ni = network.input_num();
  vector<ymuint> pat1(ni);
  vector<ymuint> pat2(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    pat1[i] = state[i * 2 + 0];
    pat2[i] = state[i * 2 + 1];
  }
  return count(network, pat1, pat2);
}

ymuint
sa(const BdnMgr& network,
   ymuint n_samples)
{
  RandGen rg;
  ymuint ni = network.input_num();
  vector<ymuint> state(ni * 2, 0);
  ymuint cur_c = count(network, state);

  ymuint max_c = cur_c;
  for (double T = 100.0; T > 0.000001; T *= 0.95) {
    for (ymuint l = 0; l < n_samples; ++ l) {
      ymuint pos = rg.int32() % (ni * 2);
      state[pos] ^= 1;
      ymuint c = count(network, state);
      if ( cur_c > c ) {
	double d0 = 1.0 / cur_c;
	double d1 = 1.0 / c;
	double d = d0 - d1;
	double t = exp(d / T);
	double r = rg.real1();
	if ( t < r ) {
	  // 今の動きをキャンセル
	  state[pos] ^= 1;
	}
      }
      cur_c = c;
      if ( max_c < c ) {
	max_c = c;
      }
    }
  }

  cout << "max_c = " << max_c << endl;
  return max_c;
}

END_NAMESPACE_YM

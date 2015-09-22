
/// @file count.cc
/// @brief count の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

void
simulate(const BdnMgr& network,
	 const vector<ymuint>& pat,
	 vector<ymuint>& state)
{
  // 入力にパタンをセットする．
  ymuint i = 0;
  const BdnNodeList& ilist = network.input_list();
  for (BdnNodeList::const_iterator p = ilist.begin();
       p != ilist.end(); ++ p, ++ i) {
    const BdnNode* node = *p;
    state[node->id()] = pat[i];
  }

  vector<const BdnNode*> node_list;
  network.sort(node_list);
  for (ymuint i = 0; i < node_list.size(); ++ i) {
    const BdnNode* node = node_list[i];
    ASSERT_COND( node->is_logic() );

    ymuint id0 = node->fanin0()->id();
    ymuint val0 = state[id0];
    if ( node->fanin0_inv() ) {
      val0 = 1 - val0;
    }

    ymuint id1 = node->fanin1()->id();
    ymuint val1 = state[id1];
    if ( node->fanin1_inv() ) {
      val1 = 1 - val1;
    }

    ymuint val = ( node->is_and() ) ? val0 & val1 : val0 ^ val1;
    state[node->id()] = val;
  }
}

END_NONAMESPACE

// pat1 -> pat2 の遷移におけるスイッチング回数を数える．
ymuint
count(const BdnMgr& network,
      const vector<ymuint>& pat1,
      const vector<ymuint>& pat2)
{
  ymuint max_id = network.max_node_id();
  vector<ymuint> cur_state(max_id);

  // pat1 のシミュレーションを行う．
  // 結果は cur_state に入る．
  simulate(network, pat1, cur_state);

  vector<ymuint> next_state(max_id);

  // pat2 のシミュレーションを行う．
  // 結果は next_state に入る．
  simulate(network, pat2, next_state);

  // 反転数を数える．
  ymuint c = 0;
  for (ymuint i = 0; i < max_id; ++ i) {
    if ( cur_state[i] != next_state[i] ) {
      ++ c;
    }
  }
  return c;
}

END_NAMESPACE_YM

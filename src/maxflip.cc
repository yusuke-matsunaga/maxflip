
/// @file maxflip.cc
/// @brief maxflip の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnBlifReader.h"
#include "YmNetworks/BdnIscas89Reader.h"
#include "YmNetworks/BdnNode.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM

ymuint
pure_mc(const BdnMgr& network,
	ymuint n_samples);

ymuint
sa(const BdnMgr& network,
   ymuint n_samples);

int
maxflip(int argc,
	char** argv)
{
  if ( argc != 2 ) {
    cerr << "USAGE: maxflip <blif-filename>" << endl;
    return 1;
  }

  string filename = argv[1];

  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnMgr network;

  BdnBlifReader read;
  if ( !read(filename, network) ) {
    cerr << "Error in reading " << filename << endl;
    return 1;
  }

  {
    cout << filename << endl
	 << "# of inputs  " << network.input_num() << endl
	 << "# of outputs " << network.output_num() << endl
	 << "# of nodes   " << network.lnode_num() << endl;
  }

  StopWatch timer;

  timer.start();
  pure_mc(network, 10000);
  timer.stop();
  cout << "CPU time = " << timer.time() << endl;

  timer.reset();
  timer.start();
  sa(network, 100);
  timer.stop();
  cout << "CPU time = " << timer.time() << endl;

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  return nsYm::maxflip(argc, argv);
}

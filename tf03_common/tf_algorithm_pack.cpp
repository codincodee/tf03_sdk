#include "tf_algorithm_pack.h"

#include <QDebug>

TFAlgorithmPack::TFAlgorithmPack()
{

}

bool TFAlgorithmPack::Test() {
#if 0
  mwArray in(2, 2, mxDOUBLE_CLASS, mxREAL);
  mwArray out;
  mxInt32 a[4] = {1, 2, 3, 4};

  in.SetData(a, 4);
  if (!testInitialize()) {
    return false;
  }
  try {
    test(1, out, in);
  } catch (...) {
    qDebug() << __LINE__;
  }
  testTerminate();
  return true;
#endif
#if 0
  mwArray dist(200, 3, mxDOUBLE_CLASS, mxREAL);
  mwArray realdist(200, 3, mxDOUBLE_CLASS, mxREAL);
  mwArray int0;
  mwArray int3;
  mwArray int7;

  double num1[600], num2[600];
  for (int i = 0; i < 600; ++i) {
    num1[i] = i + 1000;
    num2[i] = i + 1000;
  }
  dist.SetData(num1, 600);
  realdist.SetData(num2, 600);
  if (!generate_table_8Initialize()) {
    exit(1);
  }
  Generate_table_8(1, int0, int3, int7, dist, realdist);
  auto dim = int0.GetDimensions();
  mxUint32 dimi[2];
  dim.GetData(dimi, 2);
  qDebug() << dimi[0] << dimi[1];
#endif
  return true;
}

#include "tf_algorithm_pack.h"

#include "matrix.h"
#include "mclmcr.h"
#include "mclcppclass.h"
#include "mex.h"
#include "generate_table_8.h"
#include <QDebug>

TFAlgorithmPack::TFAlgorithmPack()
{

}

void TFAlgorithmPack::Test() {
  mwArray dist(200, 3, mxDOUBLE_CLASS, mxREAL);
  mwArray realdist(200, 3, mxDOUBLE_CLASS, mxREAL);
  mwArray int0(5000, 1, mxDOUBLE_CLASS, mxREAL);
  mwArray int3(5000, 1, mxDOUBLE_CLASS, mxREAL);
  mwArray int7(5000, 1, mxDOUBLE_CLASS, mxREAL);

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
}

#include "mini_generate_table_8.h"
#include "matlab_dependencies.h"
#include <tf_algorithms/generate_table_8.h>

MiniGenerateTable8::MiniGenerateTable8()
{

}

bool MiniGenerateTable8::GenerateWithoutException(
    const Int037Input &dist,
    const Int037Input &realdist,
    std::vector<mxInt32> &int0,
    std::vector<mxInt32> &int3,
    std::vector<mxInt32> &int7) {
  bool success = false;
  try {
    success = Generate(dist, realdist, int0, int3, int7);
  } catch (...) {
    success = false;
  }
  return success;
}

bool MiniGenerateTable8::Generate(
    const Int037Input& dist, const Int037Input& realdist,
    std::vector<mxInt32>& int0,
    std::vector<mxInt32>& int3,
    std::vector<mxInt32>& int7) {
  auto row_len = dist.i0.size();
  if (row_len == 0) {
    return false;
  }
  if (dist.i3.size() != row_len || dist.i7.size() != row_len) {
    return false;
  }
  if (realdist.i0.size() != row_len ||
      realdist.i3.size() != row_len ||
      realdist.i7.size() != row_len) {
    return false;
  }

  if (!generate_table_8Initialize()) {
    return false;
  }

  std::vector<mxInt32*> recycle;

  mwArray mdist(row_len, 3, mxINT32_CLASS, mxREAL);
  mxInt32* mdist_arr = new mxInt32[row_len * 3];
  recycle.push_back(mdist_arr);
  int index = 0;
  for (int i = 0; i < row_len; ++i) {
    mdist_arr[index++] = dist.i0[i];
  }
  for (int i = 0; i < row_len; ++i) {
    mdist_arr[index++] = dist.i3[i];
  }
  for (int i = 0; i < row_len; ++i) {
    mdist_arr[index++] = dist.i7[i];
  }
  mdist.SetData(mdist_arr, row_len * 3);

  mwArray mrealdist(row_len, 3, mxINT32_CLASS, mxREAL);
  mxInt32* mrealdist_arr = new mxInt32[row_len * 3];
  recycle.push_back(mrealdist_arr);
  index = 0;
  for (int i = 0; i < row_len; ++i) {
    mrealdist_arr[index++] = realdist.i0[i];
  }
  for (int i = 0; i < row_len; ++i) {
    mrealdist_arr[index++] = realdist.i3[i];
  }
  for (int i = 0; i < row_len; ++i) {
    mrealdist_arr[index++] = realdist.i7[i];
  }
  mrealdist.SetData(mrealdist_arr, row_len * 3);

  mwArray mint0;
  mwArray mint3;
  mwArray mint7;

  Generate_table_8(1, mint0, mint3, mint7, mdist, mrealdist);

  int0.clear();
  int3.clear();
  int7.clear();

  mxUint32 mdim_arr[2];

  auto mdim = mint0.GetDimensions();
  mdim.GetData(mdim_arr, 2);
  int int0_len = mdim_arr[0];
  if (int0_len == 0) {
    return false;
  }
  mxInt32* mint0_arr = new mxInt32[int0_len];
  recycle.push_back(mint0_arr);
  mint0.GetData(mint0_arr, int0_len);
  int0.reserve(int0_len);
  for (int i = 0; i < int0_len; ++i) {
    int0.push_back(mint0_arr[i]);
  }

  mdim = mint3.GetDimensions();
  mdim.GetData(mdim_arr, 2);
  int int3_len = mdim_arr[0];
  if (int3_len == 0) {
    return false;
  }
  mxInt32* mint3_arr = new mxInt32[int3_len];
  recycle.push_back(mint3_arr);
  mint3.GetData(mint3_arr, int3_len);
  int3.reserve(int3_len);
  for (int i = 0; i < int3_len; ++i) {
    int3.push_back(mint3_arr[i]);
  }

  mdim = mint7.GetDimensions();
  mdim.GetData(mdim_arr, 2);
  int int7_len = mdim_arr[0];
  if (int7_len == 0) {
    return false;
  }
  mxInt32* mint7_arr = new mxInt32[int7_len];
  recycle.push_back(mint7_arr);
  mint7.GetData(mint7_arr, int7_len);
  int7.reserve(int7_len);
  for (int i = 0; i < int7_len; ++i) {
    int7.push_back(mint7_arr[i]);
  }

  generate_table_8Terminate();

  for (auto& a : recycle) {
    delete [] a;
  }
  return true;
}

int MiniGenerateTable8::TempCompensateInt0(
    const int &refdist, const int& realdist, const int &temp) {
  const float P1[] = {
      -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f,
      -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f,
      -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f, -31.08f};

  const float P2[] = {
      -350.56f, -350.88f, -351.21f, -351.53f, -351.86f, -352.18f, -352.50f, -352.83f, -353.15f, -353.47f,
      -353.80f, -354.12f, -354.44f, -354.77f, -355.09f, -355.41f, -355.74f, -356.06f, -356.39f, -356.71f,
      -357.03f, -357.36f, -357.68f, -358.00f, -358.33f, -358.65f, -358.97f, -359.30f, -359.62f, -359.94f};

  const float P3[] = {
      246.87f, 245.84f, 244.82f, 243.80f, 242.77f, 241.75f, 240.73f, 239.70f, 238.68f, 237.66f,
      236.63f, 235.61f, 234.59f, 233.56f, 232.54f, 231.51f, 230.49f, 229.47f, 228.44f, 227.42f,
      226.40f, 225.37f, 224.35f, 223.33f, 222.30f, 221.28f, 220.26f, 219.23f, 218.21f, 217.19f};

  const int table_len = 30;
  const int refdist_limit[] = {1000, 2160};
  const int step = 40;
  double x = refdist;
  double y = (temp - 1112) / 75.34;
  double idx_now = (x - refdist_limit[0]) / step;
  int idx_down = floor(idx_now);
  int idx_up = idx_down + 1;
  int idx;
  bool flag;
  if (idx_now <= 0) {
    idx = 0;
    flag = true;
  } else if (idx_now > (table_len - 1)) {
    idx = table_len - 1;
    flag = true;
  } else {
    flag = false;
  }
  double z;
  if (flag) {
    z = P1[idx] * pow(y, 2) + P2[idx] * y + P3[idx];
  } else {
    double z1 = P1[idx_down] * pow(y, 2) + P2[idx_down] * y + P3[idx_down];
    double z2 = P1[idx_up] * pow(y, 2) + P2[idx_up] * y + P3[idx_up];
    z = z1 + (z2 - z1) * (idx_now - idx_down);
  }
  return round(realdist - z);
}

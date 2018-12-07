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
  const double P1[] = {
      -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08,
      -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08,
      -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08, -31.08f};

  const double P2[] = {
      -350.56, -350.88, -351.21, -351.53, -351.86, -352.18, -352.50, -352.83, -353.15, -353.47,
      -353.80, -354.12, -354.44, -354.77, -355.09, -355.41, -355.74, -356.06, -356.39, -356.71,
      -357.03, -357.36, -357.68, -358.00, -358.33, -358.65, -358.97, -359.30, -359.62, -359.94f};

  const double P3[] = {
      246.87, 245.84, 244.82, 243.80, 242.77, 241.75, 240.73, 239.70, 238.68, 237.66,
      236.63, 235.61, 234.59, 233.56, 232.54, 231.51, 230.49, 229.47, 228.44, 227.42,
      226.40, 225.37, 224.35, 223.33, 222.30, 221.28, 220.26, 219.23, 218.21, 217.19f};

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

int MiniGenerateTable8::TempCompensateInt3(
    const int &refdist, const int &realdist, const int &temp) {
  const double P1[] = {
      -40.7619, -41.2954, -44.4246, -43.5328, -44.3125, -42.9678, -42.9639, -40.1885, -37.6192, -37.6811,
      -38.5339, -42.7371, -43.7811, -44.6743, -45.0953, -45.7362, -45.4780, -51.2557, -51.9798, -52.9332,
      -54.1860, -51.7197, -45.6269, -42.2476, -44.8393, -44.3517, -47.0002, -53.0048, -65.5877, -69.7432,
      -73.4904, -78.0640, -81.7901, -78.8396, -79.5862, -81.5544, -97.3915, -104.9589, -88.8683, -88.8683
  };

  const double P2[] = {
      4.5131, 4.6794, 5.3857, 5.2063, 5.3378, 4.9689, 4.9157, 4.2880, 3.6458, 3.5741,
      3.7254, 4.6700, 4.9419, 5.2152, 5.3891, 5.6367, 5.6911, 7.0269, 7.1325, 7.2771,
      7.4858, 6.9130, 5.6598, 4.8622, 5.3355, 5.1563, 5.7361, 7.0697, 9.8781, 10.8238,
      11.7118, 12.7584, 13.6730, 13.0842, 13.3298, 13.7866, 17.5462, 19.4482, 16.0692, 16.0692
  };

  const double P3[] = {
      195.2287, 74.3818, -325.8788, -238.8793, -289.7064, -43.9367, 16.1119, 370.6285, 768.6461, 858.2390,
      796.6114, 265.1829, 89.7578, -106.3374, -250.2201, -451.7011, -548.7590, -1321.5663, -1349.1018, -1388.8095,
      -1462.2637, -1133.3058, -495.7442, -36.7035, -230.1069, -86.3333, -406.4713, -1147.2959, -2717.1312, -3254.6952,
      -3776.8131, -4382.4752, -4939.5103, -4660.7761, -4831.6752, -5100.7863, -7330.9982, -8526.7653, -6775.8166, -6775.8166
  };

  const int table_len = 40;
  const int refdist_limit[] = {1200, 3100};
  const int step = 50;

  double x = refdist;
  double y = temp;

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
    z = P1[idx] * pow(y, 2) / 1000 + P2[idx] * y + P3[idx];
  } else {
    double z1 = P1[idx_down] * pow(y, 2) / 1000 + P2[idx_down] * y + P3[idx_down];
    double z2 = P1[idx_up] * pow(y, 2) / 1000 + P2[idx_up] * y + P3[idx_up];
    z = z1 + (z2 - z1) * (idx_now - idx_down);
  }
  return std::round(realdist - z);
}

int MiniGenerateTable8::TempCompensateInt7(
    const int &refdist, const int &realdist, const int &temp) {
  const double P1[] = {
      -15.9843, -36.3757, -35.1459, -28.1603, -30.8058, -30.3798, -32.3098, -34.4536, -35.3027, -37.0245,
      -38.4523, -39.6736, -40.4720, -42.0234, -42.1274, -43.8202, -43.9883, -45.1924, -46.1799, -47.1869,
      -47.3741, -47.4565, -47.3942, -47.9076, -48.3370, -48.6260, -49.6617, -50.4941, -51.3239, -52.4025,
      -53.0157, -52.0025, -53.1145, -52.0217, -51.1247, -51.1885, -50.4340, -49.9510, -48.3618, -49.0678,
      -48.3747, -47.8387, -48.2947, -47.7403, -46.6337, -46.6489, -46.2410, -45.3391, -44.8277, -45.6461,
      -44.4889, -44.6668, -44.3327, -44.6707, -44.5116, -45.6551, -46.4890, -45.6685, -45.5782, -45.2664,
      -45.4164, -44.2916, -45.5918, -44.5051, -46.0113, -44.0876, -45.0989, -46.7417, -49.1000, -47.3779,
      -46.9388, -50.1060, -48.3504, -48.1894, -47.7835, -49.2225, -48.3204, -50.1412, -52.6692, -50.6565,
      -52.0105, -52.6865, -53.4083, -53.8388, -52.6062, -53.4269, -52.9794, -55.6466, -57.1558, -56.6833,
      -56.7590, -56.2259, -57.0762, -54.3244, -54.7829, -53.6904, -52.3550, -53.7547, -51.2240, -57.1924,
      -58.7739, -65.8257, -72.7323, -76.1101, -76.1101, -76.1101, -76.1101, -76.1101, -76.1101, -76.1101
  };

  const double P2[] = {
      -1.1704, 3.4489, 3.2594, 1.7273, 2.3384, 2.2267, 2.6473, 3.1089, 3.2806, 3.6414,
      3.9438, 4.2060, 4.3667, 4.7063, 4.7281, 5.1111, 5.1465, 5.4157, 5.6349, 5.8539,
      5.8835, 5.8905, 5.8547, 5.9538, 6.0330, 6.0864, 6.3168, 6.5023, 6.7043, 6.9636,
      7.1216, 6.9145, 7.1998, 6.9774, 6.8052, 6.8453, 6.7114, 6.6334, 6.3066, 6.4999,
      6.3741, 6.2836, 6.4166, 6.3136, 6.0858, 6.1088, 6.0341, 5.8384, 5.7208, 5.8970,
      5.6147, 5.6336, 5.5166, 5.5531, 5.4693, 5.6731, 5.7984, 5.5581, 5.4727, 5.3376,
      5.3107, 4.9984, 5.2363, 4.9499, 5.2677, 4.8221, 5.0500, 5.4368, 5.9854, 5.6200,
      5.5557, 6.3055, 5.9459, 5.9461, 5.9044, 6.2830, 6.1194, 6.5759, 7.2091, 6.7977,
      7.1431, 7.3534, 7.5536, 7.7121, 7.4632, 7.6717, 7.5883, 8.1962, 8.5230, 8.3875,
      8.3829, 8.2128, 8.3622, 7.7035, 7.7597, 7.4733, 7.1154, 7.3863, 6.7533, 8.0662,
      8.4172, 10.0379, 11.6272, 12.4358, 12.4358, 12.4358, 12.4358, 12.4358, 12.4358, 12.4358
  };

  const double P3[] = {
      3518.9749, 932.9413, 990.8498, 1827.3787, 1475.7080, 1547.9797, 1320.2389, 1073.5372, 987.6484, 799.5663,
      640.2991, 499.8843, 419.4019, 232.8447, 220.6547, 3.2423, -16.9626, -167.6878, -289.9577, -409.0622,
      -418.7795, -416.1469, -382.9817, -428.3357, -461.8759, -484.8396, -613.7762, -717.0941, -840.1184, -995.8639,
      -1096.8936, -993.8172, -1175.0177, -1064.8661, -985.8416, -1024.1191, -970.4844, -945.6106, -780.9104, -910.1164,
      -857.3635, -825.8175, -919.0804, -873.6434, -759.3384, -784.5404, -752.4094, -647.3634, -579.6962, -673.9349,
      -502.6413, -500.2811, -409.4043, -405.9344, -330.4644, -414.1733, -446.2158, -276.9813, -191.0624, -77.3954,
      -24.5910, 185.6597, 84.8258, 272.0112, 105.5850, 362.7580, 233.8532, 6.5379, -312.6546, -121.4960,
      -106.7170, -547.7813, -368.1832, -391.8683, -397.4657, -643.9585, -575.9970, -860.7760, -1255.3353, -1049.5080,
      -1267.4805, -1422.5002, -1557.6367, -1682.8934, -1561.3235, -1693.5806, -1656.1640, -2002.8770, -2179.5672, -2087.3640,
      -2071.9168, -1947.7525, -2006.3565, -1615.1282, -1616.9539, -1432.4626, -1200.0792, -1323.6253, -931.6360, -1654.0799,
      -1849.5010, -2781.4510, -3698.6383, -4182.0887, -4182.0887, -4182.0887, -4182.0887, -4182.0887, -4182.0887, -4182.0887
  };

  const int table_len = 110;
  const int refdist_limit[] = {1150, 3210};
  const int step = 20;

  const double x = refdist;
  const double y = temp;

  double idx_now = (x - refdist_limit[0]) / step;
  int idx_down = floor(idx_now);
  int idx_up = idx_down + 1;
  int idx;
  bool flag;
  if (idx_now <= 0) {
    idx = 0;
    flag = true;
  } else if (idx_now >= (table_len - 1)) {
    idx = table_len - 1;
    flag = true;
  } else {
    flag = false;
  }

  double z;
  if (flag) {
    z = P1[idx] * pow(y, 2) / 1000 + P2[idx] * y + P3[idx];
  } else {
    double z1 = P1[idx_down] * pow(y, 2) / 1000 + P2[idx_down] * y + P3[idx_down];
    double z2 = P1[idx_up] * pow(y, 2) / 1000 + P2[idx_up] * y + P3[idx_up];
    z = z1 + (z2 - z1) * (idx_now - idx_down);
  }
  return std::round(realdist - z);
}

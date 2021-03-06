#ifndef MINI_GENERATE_TABLE_8_H
#define MINI_GENERATE_TABLE_8_H

#include <stdint.h>
#include <vector>
#include "matlab_dependencies.h"

struct Int037Input {
  std::vector<mxInt32> i0;
  std::vector<mxInt32> i3;
  std::vector<mxInt32> i7;
};

class MiniGenerateTable8
{
public:
  MiniGenerateTable8();
  static bool Generate(
      const Int037Input& dist,
      const Int037Input& realdist,
      const Int037Input& amp,
      std::vector<mxInt32>& int0,
      std::vector<mxInt32>& int3,
      std::vector<mxInt32>& int7);
  static bool GenerateWithoutException(
      const Int037Input &dist,
      const Int037Input &realdist,
      const Int037Input& amp,
      std::vector<mxInt32> &int0,
      std::vector<mxInt32> &int3,
      std::vector<mxInt32> &int7);
  static std::vector<uint16_t> ConvertToDriverTable(
      const std::vector<mxInt32>& table);
  static int TempCompensateInt0(
      const int& refdist, const int& realdist, const int& temp);
  static int TempCompensateInt3(
      const int& refdist, const int& realdist, const int& temp);
  static int TempCompensateInt7(
      const int& refdist, const int& realdist, const int& temp);
  static void TrimInput(Int037Input& input);
};

#endif // MINI_GENERATE_TABLE_8_H

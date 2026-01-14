#pragma once

class ConfigImage 
{
 public:
  inline static float lsR = 0.33f;
  inline static float lsG = 0.60f;
  inline static float lsB = 0.03f;
};


class KeblerImage
{
public:
  inline static float leftSobel[9] = { -1, 0, 1,
                         -2, 0, 2,
                         -1, 0, 1 };

  inline static float topSobel[9] = { 1, 2, 1,
                        0, 0, 0,
                        -1, -2, -1 };

  inline static float leftPrewwit[9] = { 1, 0, -1,
                           1, 0, -1,
                           1, 0, -1 };

  inline static float topPrewwit[9] = { 1, 1, 1,
                          0, 0, 0,
                          -1, -1, -1 };

  inline static float blur[9] = { 0.0625f, 0.125f, 0.0625f,
                    0.125f, 0.25f, 0.125f,
                    0.0625f, 0.125f, 0.0625f };

  inline static float outLine[9] = { -1, -1, -1,
                       -1, 8, -1,
                       -1, -1, -1 };
};
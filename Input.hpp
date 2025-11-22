#pragma once

class Input
{
public:
  bool *keys;

  Input()
  {
    keys = new bool[256];

    for (int i = 0; i < 256; i++)
    {
      keys[i] = false;
    }
  }

  ~Input()
  {
    delete[] keys;
  }
};

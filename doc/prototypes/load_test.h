#ifndef _LOAD_TEST_H_
#define _LOAD_TEST_H_

#include <string>
#include <vector>
#include <iostream>

struct Dummy
{
  void declare (char * name)
  {
    mList.push_back(std::string(name));
  }
  
  void print()
  {
    int i;
    for(i = 0;i<mList.size();i++) {
      std::cout << i << " : " << mList[i] << std::endl;
    }
  }
  
  std::vector<std::string> mList;
};

#endif
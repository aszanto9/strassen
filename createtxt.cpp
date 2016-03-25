#include <iostream>
#include <set>
#include <thread>
#include <vector>
#include <ctime>
#include <cmath>
#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <random>
#include <algorithm>

using namespace std;

int main() {

  ofstream outFile("sum.txt");
  for (int i = 0; i < pow(256,2)*2; ++i)
  {
     outFile << 1 << endl;
  }  
  outFile.close();
  return 0;
}
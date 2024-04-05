#include "autocomplete.h"
#include <algorithm>
#include <fstream>

using namespace std;

void testBSTAll();

void Autocomplete::readFile(const string &fileName) {
  ifstream ifs(fileName);
  // TODO(student)
  string str;
  int fileSize = 0;
  vector<BSTMap::value_type> vec;
  BSTMap::value_type temp;
  ifs >> fileSize;
  for (int i = 0; i < fileSize; i++) {
    ifs >> temp.second >> temp.first;
    getline(ifs, str);
    temp.first.append(str);
    vec.push_back(temp);
  }
  phrases.insertVector(vec);

  ifs.close();
  phrases.rebalance();
  // cout << phrases << endl;
}

bool Autocomplete::sortByWeight(BSTMap::value_type &a, BSTMap::value_type &b) {
  return a.second > b.second;
}

vector<BSTMap::value_type>
Autocomplete::complete(const BSTMap::key_type &prefix) const {
  vector<BSTMap::value_type> v;
  // TODO(student)
  v = phrases.getAll(prefix);
  sort(v.begin(), v.end(), sortByWeight);
  return v;
}

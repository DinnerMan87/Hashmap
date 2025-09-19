#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(HashMapCore, GeneralCoreTests) {
  HashMap mapa = HashMap<int,int>();
  EXPECT_THAT(mapa.empty(), Eq(true));
  EXPECT_THAT(mapa.size(), Eq(0));

  map<int,int> reference;
  for(int i = 0; i < 5; i++){
    mapa.insert(i,i);
    reference[i] = i;
    EXPECT_THAT(mapa.at(i), Eq(reference[i]))
    << "Map at key " << i << ": " << mapa.at(i);
  }
  EXPECT_THAT(mapa.empty(), Eq(false));
  EXPECT_THAT(mapa.size(), Eq(5));
  ASSERT_THROW(mapa.at(-1),out_of_range)
  << "Map at key -1 is out of range" << endl;

  EXPECT_THAT(mapa.contains(0), Eq(true))
  << "Map doesn't contain 0 when it should" << endl;
  EXPECT_THAT(mapa.contains(3), Eq(true))
  << "Contains function is running false for 3 when it should be true" << endl;
  EXPECT_THAT(mapa.contains(-1), Eq(false))
  << "Contains function is running true for -1 when it should be false" << endl;

  mapa.insert(0,-1);
  EXPECT_THAT(mapa.at(0), Eq(reference[0]))
  << "Map at key 0 is " << mapa.at(0);
  
  mapa.clear();
  EXPECT_THAT(mapa.empty(), Eq(true));
  EXPECT_THAT(mapa.size(), Eq(0));
  ASSERT_THROW(mapa.at(0),out_of_range)
  << "Map at key -1 is out of range" << endl;

  HashMap mp1 = HashMap<int,int>();
  mp1.insert(0,0);
  mp1.insert(1,1);
  mp1.insert(2,2);
  HashMap mp2 = mp1;
  for(int i = 0; i < 3; i++){ 
    EXPECT_THAT(mp2.at(i), Eq(i))
    << "data at " << i << " is \"" << mp2.at(i) << "\"" << endl;
  }
  mp2.clear(); 
  int mpsize = mp1.size();
  EXPECT_THAT(mpsize, Eq(3))
  << "size=\"" << mpsize << "\"" << endl;
  int isEmpty = mp1.empty();
  EXPECT_THAT(isEmpty, Eq(false))
  << "isEmpty=\"" << isEmpty << "\"" << endl;
  mpsize = mp2.size();
  EXPECT_THAT(mpsize, Eq(0))
  << "size=\"" << mpsize << "\"" << endl;
  isEmpty = mp2.empty();
  EXPECT_THAT(isEmpty, Eq(true))
  << "isEmpty=\"" << isEmpty << "\"" << endl;

  HashMap<int,int> mp3 = mp2;
  mpsize = mp3.size();
  EXPECT_THAT(mpsize, Eq(0))
  << "size=\"" << mpsize << "\"" << endl;
  isEmpty = mp3.empty();
  EXPECT_THAT(isEmpty, Eq(true))
  << "isEmpty=\"" << isEmpty << "\"" << endl;

  mp3.insert(0,0);
  mp3.insert(1,1);
  mp3.insert(2,2);
  mp3 = mp2;
  mpsize = mp3.size();
  EXPECT_THAT(mpsize, Eq(0))
  << "size=\"" << mpsize << "\"" << endl;
  isEmpty = mp3.empty();
  EXPECT_THAT(isEmpty, Eq(true))
  << "isEmpty=\"" << isEmpty << "\"" << endl;

  mp2 = mp1;
  for(int i = 0; i < 3; i++){ 
    EXPECT_THAT(mp2.at(i), Eq(i))
    << "data at " << i << " is \"" << mp2.at(i) << "\"" << endl;
  }
  isEmpty = mp2.empty();
  EXPECT_THAT(isEmpty, Eq(false))
  << "isEmpty=\"" << isEmpty << "\"" << endl;

  HashMap<int,int>& mp1ref = mp1;
  mp1 = mp1ref;
  for(int i = 0; i < 3; i++){ 
    EXPECT_THAT(mp1.at(i), Eq(i))
    << "data at " << i << " is \"" << mp1.at(i) << "\"" << endl;
  }
}

TEST(HashMapCore, HashMapCoreTests) {
  HashMap<int,int> mapb = HashMap<int,int>(5);
  map<int,int> reference;
  for(int i = 0; i < 15; i++){
    mapb.insert(i,i);
    reference[i] = i;
    EXPECT_THAT(mapb.at(i), Eq(reference[i]))
    << "Map at key " << i << ": " << mapb.at(i);
  }

  for(int i = 5; i < 15; i++){ //collisions
    EXPECT_THAT(mapb.at(i), Eq(reference[i]))
    << "Map at key " << i << ": " << mapb.at(i);
  }

  ASSERT_THROW(mapb.erase(-1),out_of_range)
  << "No key -1" << endl;

  mapb.erase(0);
  for(int i = 1; i < 15; i++){
    EXPECT_THAT(mapb.at(i), Eq(reference[i]))
    << "Map at key " << i << ": " << mapb.at(i);
  }

  mapb.erase(10); //collisions
  for(int i = 1; i < 15; i++){
    if(i != 10){
      EXPECT_THAT(mapb.at(i), Eq(reference[i]))
      << "Map at key " << i << ": " << mapb.at(i);
    }
  }
} 

TEST(HashMapCore, ExtraCoreTests) {
  HashMap<int,int> mapc = HashMap<int,int>(5);
  map<int,int> reference;
  for(int i = 0; i < 1000; i++){
    mapc.insert(i,i);
    reference[i] = i;
    EXPECT_THAT(mapc.at(i), Eq(reference[i]))
    << "Map at key " << i << ": " << mapc.at(i);
  }
  for(int i = 0; i < 1000; i++){
    EXPECT_THAT(mapc.contains(i),true);
  }
  mapc.at(100);

  HashMap<int,int> mapd = HashMap<int,int>(5);
  map<int,int> reference2;
  for(int i = 0; i < 1000; i++){
    int key = Random::randInt(1000);
    mapd.insert(key, key);
    reference2[key]=key;
    EXPECT_THAT(mapd.at(key), Eq(reference2[key]))
    << "Map at key " << key << ": " << mapd.at(key);
    int rand = Random::randInt(1000);
    if(rand >= key){
      mapd.erase(key);
    }
  }
  
}
}  // namespace

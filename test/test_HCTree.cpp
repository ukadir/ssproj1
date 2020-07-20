#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        tree.build(freqs);
        // cout << " tree builds " << endl;
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    // cout << " a encodes to " << os.str() << endl;
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    ASSERT_EQ(tree.decode(is), 'b');
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE_SAME_FREQ) {
    vector<unsigned int> freqs(256);
    ostringstream os;
    freqs['a'] = 2;
    freqs['b'] = 2;
    tree.build(freqs);

    tree.encode('b', os);
    ASSERT_EQ(os.str(), "0");
}

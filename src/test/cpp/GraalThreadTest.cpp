#include "ClientBase.h"
#include <gtest/gtest.h>

using namespace ons;

class SampleClient : ClientBase {
public:
    SampleClient() {

    }

    ~SampleClient() {

    }

};

TEST(GraalThreadTest, testAttach_Detach) {
    SampleClient *sample = new SampleClient();
    delete sample;
}
#include "gtest/gtest.h"
#include <cmath>

#include "cuBERT/common.h"
#include "cuBERT/op/LayerNorm.h"
using namespace cuBERT;

class LayerNormTest : public ::testing::Test {
protected:
    void SetUp() override {
        cuBERT::initialize();
    }

    void TearDown() override {
        cuBERT::finalize();
    }
};

TEST_F(LayerNormTest, compute_) {
    float beta[3] = {-1, 0, 1};
    float gamma[3] = {1, 2, 3};

    size_t max_batch_size = 2;
    LayerNorm layer_norm(max_batch_size, 3, beta, gamma);

    float inout[6] = {9, 10, 11, 5, 4, 3};

    float* inout_gpu = (float*) cuBERT::malloc(6 * sizeof(float));
    cuBERT::memcpy(inout_gpu, inout, 6 * sizeof(float), 1);

    layer_norm.compute_(2, inout_gpu, nullptr);

    cuBERT::memcpy(inout, inout_gpu, 6 * sizeof(float), 2);
    cuBERT::free(inout_gpu);

    EXPECT_NEAR(inout[0], -2.224744871391589, 1e-5);
    EXPECT_FLOAT_EQ(inout[1], 0);
    EXPECT_NEAR(inout[2], 4.674234614174766, 1e-5);
    EXPECT_NEAR(inout[3], 0.22474487139158894, 1e-5);
    EXPECT_FLOAT_EQ(inout[4], 0);
    EXPECT_NEAR(inout[5], -2.674234614174767, 1e-5);
}


class LayerNormCPUTest : public ::testing::Test {
protected:
    void SetUp() override {
        cuBERT::initialize(true);
    }

    void TearDown() override {
        cuBERT::finalize();
    }
};

TEST_F(LayerNormCPUTest, compute_cpu_) {
    float beta[3] = {-1, 0, 1};
    float gamma[3] = {1, 2, 3};

    size_t max_batch_size = 2;
    LayerNorm layer_norm(max_batch_size, 3, beta, gamma);

    float inout[6] = {9, 10, 11, 5, 4, 3};
    layer_norm.compute_(2, inout, nullptr);

    EXPECT_NEAR(inout[0], -2.224744871391589, 1e-5);
    EXPECT_FLOAT_EQ(inout[1], 0);
    EXPECT_NEAR(inout[2], 4.674234614174766, 1e-5);
    EXPECT_NEAR(inout[3], 0.22474487139158894, 1e-5);
    EXPECT_FLOAT_EQ(inout[4], 0);
    EXPECT_NEAR(inout[5], -2.674234614174767, 1e-5);
}

TEST_F(LayerNormCPUTest, compute_cpu_ext_) {
    float beta[3] = {-1, 0, 1};
    float gamma[3] = {1, 2, 3};

    size_t max_batch_size = 2;
    LayerNorm layer_norm(max_batch_size, 3, beta, gamma);

    float in[6] = {8, 8, 8, 2, 2, 2};
    float inout[6] = {1, 2, 3, 3, 2, 1};
    layer_norm.compute_(2, in, inout, nullptr);

    EXPECT_NEAR(inout[0], -2.224744871391589, 1e-5);
    EXPECT_FLOAT_EQ(inout[1], 0);
    EXPECT_NEAR(inout[2], 4.674234614174766, 1e-5);
    EXPECT_NEAR(inout[3], 0.22474487139158894, 1e-5);
    EXPECT_FLOAT_EQ(inout[4], 0);
    EXPECT_NEAR(inout[5], -2.674234614174767, 1e-5);
}

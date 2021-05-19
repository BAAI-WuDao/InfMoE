#pragma once

#ifndef SUBLAYER_H
#define SUBLAYER_H

#include <NvInferPlugin.h>
#include <cublas.h>

#include <cassert>

#include "utility.h"

using nvinfer1::Dims;
using nvinfer1::Weights;

class MoESubLayer {
   protected:
    int mExpertCount;
    int mHiddenSize;
    int mMaxConcurrency;
    const char *mWeightFile;
    cublasHandle_t *mCublasHandle = nullptr; // passed by MoELayerPlugin

   public:
    explicit MoESubLayer(int expertCount, int hiddenSize, const char *weightFile, int maxConcurrency, cublasHandle_t *cublasHandle)
        : mExpertCount(expertCount), mHiddenSize(hiddenSize), mWeightFile(weightFile), mMaxConcurrency(maxConcurrency), mCublasHandle(cublasHandle){};
    virtual ~MoESubLayer(){};
    virtual bool init(const Dims *inputDims, int32_t nbInputs, const Dims *outputDims, int32_t nbOutputs) = 0;
    virtual size_t weightSize() = 0;
    virtual size_t workspaceSize(int32_t maxBatchSize) = 0;
    virtual Dims getOutputDimensions(int32_t index, const Dims *inputs, int32_t nbInputDims) = 0;
    virtual void copyWeights(void *dst, int expert, cudaStream_t stream) = 0;
    virtual bool run(int32_t batchSize, const void *weights, const void *const *inputs, void **outputs, void *workspace,
                     cudaStream_t stream) = 0;
    // read weights to memory, etc.
    virtual void initialize() = 0;
    // free weights, etc.
    virtual void terminate() = 0;
};

#endif  // SUBLAYER_H

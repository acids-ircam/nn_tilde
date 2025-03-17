#pragma once
#include <vector>
#include <torch/torch.h>


template <typename data_type> 
class StaticBuffer {

public:
    using BufferData = std::vector<data_type>;
    using BufferShape = std::array<size_t, 2>;

private:
    std::shared_ptr<BufferData> _data; 
    BufferShape _dims;
    double _samplingRate; 
    
public: 

    StaticBuffer() {
        _dims = BufferShape({0, 0});
    };

    StaticBuffer(const size_t dim1, const size_t dim2, const double samplingRate = -1) {
        _dims = BufferShape({dim1, dim2});
        _data = std::make_shared<BufferData>(get_empty_buffer(dim1, dim2));
        _samplingRate = samplingRate;
    };

    StaticBuffer(BufferData data, const double samplingRate) {
        _dims = BufferShape({data.dims(), data[0].dims()});
        _data = std::make_shared(data);
        _samplingRate = samplingRate;
    };

    StaticBuffer(const StaticBuffer &buffer) {
        _dims = buffer._dims; 
        _data = buffer._data;
        _samplingRate = buffer._samplingRate;
    }

    BufferShape dims () {
        return _dims;
    }

    double sr() { return _samplingRate; }

    static BufferData get_empty_buffer(const size_t dim1, const size_t dim2) {
        return BufferData(dim1 * dim2, 0.f);
    };

    void clear() {
        _data.get()->clear();
    }

    void reset() {
        std::fill(_data.get()->begin(), _data.get()->end(), 0.0f);
    };

    torch::Tensor to_tensor() {
        auto obj = torch::from_blob(_data.get()->data(), {(long long)_dims[0], (long long)_dims[1]}, torch::kFloat);
        return obj;
    };

    float& at(const size_t dim1, const size_t dim2) {
        return _data.get()->at(dim1 * _dims[0] + dim2);
    }

    void put(data_type data, const size_t dim1, const size_t dim2) {
        at(dim1, dim2) = data;
    }
};
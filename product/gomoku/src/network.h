#pragma once
#include "game.h"
#include <mxnet-cpp/MxNetCpp.h>

struct SampleData {
public:
    void flip_verticing();
    void transpose();
    friend std::ostream& operator<<(std::ostream& out, const SampleData& sample);

public:
    float data[INPUT_FEATURE_NUM * BOARD_SIZE] = { 0.0f };
    float p_label[BOARD_SIZE] = { 0.0f };
    float v_label[1] = { 0.0f };
};

struct MiniBatch {
public:
    friend std::ostream& operator<<(std::ostream& out, const MiniBatch& batch);

public:
    float data[BATCH_SIZE * INPUT_FEATURE_NUM * BOARD_SIZE] = { 0.0f };
    float p_label[BATCH_SIZE * BOARD_SIZE] = { 0.0f };
    float v_label[BATCH_SIZE * 1] = { 0.0f };
};

class DataSet {
public:
    DataSet()
        : index(0)
    {
        buf = new SampleData[BUFFER_SIZE];
    }
    ~DataSet() { delete[] buf; }
    int size() const { return (index > BUFFER_SIZE) ? BUFFER_SIZE : index; }
    long long total() const { return index; }
    void push_with_transform(SampleData* data);
    void make_mini_batch(MiniBatch* batch) const;

    void push_back(const SampleData* data)
    {
        buf[index % BUFFER_SIZE] = *data;
        ++index;
    }

    const SampleData& get(int i) const
    {
        assert(i < size());
        return buf[i];
    }

    friend std::ostream& operator<<(std::ostream& out, const DataSet& set)
    {
        for (int i = 0; i < set.size(); ++i)
            out << set.get(i) << std::endl;
        return out;
    }

private:
    long long index;
    SampleData* buf;
};

class FIRNet {
    using Symbol = mxnet::cpp::Symbol;
    using Context = mxnet::cpp::Context;
    using NDArray = mxnet::cpp::NDArray;
    using Executor = mxnet::cpp::Executor;
    using Optimizer = mxnet::cpp::Optimizer;

public:
    FIRNet(long long verno);
    ~FIRNet();
    long long verno() { return update_cnt; }
    void save_param();
    void show_param(std::ostream& out);
    float train_step(const MiniBatch* batch);
    void forward(
        const State& state,
        float value[1],
        std::vector<std::pair<Move, float>>& move_priors);

private:
    void build_graph();
    void bind_train();
    void bind_predict();
    void init_param();
    void load_param();
    void adjust_lr();
    float calc_init_lr();
    std::string make_param_file_name();

    long long update_cnt;
    const Context ctx;
    std::map<std::string, NDArray> args_map;
    std::map<std::string, NDArray> auxs_map;
    std::vector<std::string> loss_arg_names;
    Symbol plc, val, loss;
    NDArray data_predict, data_train, plc_label, val_label;
    Executor *plc_predict, *val_predict, *loss_train;
    Optimizer* optimizer;
};
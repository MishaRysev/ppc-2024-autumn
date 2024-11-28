// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <vector>

#include "mpi/rysev_m_gypercube/include/ops_mpi.hpp"

TEST(rysev_m_gypercube, data_transfer) {
  boost::mpi::communicator world;

  std::vector<uint8_t> in(10);
  std::vector<uint8_t> out(10);
  for (uint8_t i = 0; i < in.size(); i++) {
    in[i] = i;
  }

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  rysev_m_gypercube::GyperCube task(taskDataPar);
  ASSERT_TRUE(task.validation());
  task.pre_processing();
  ASSERT_TRUE(task.run());
  task.post_processing();

  for (uint8_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], in[i]);
  }
}

TEST(t_rysev_m_gypercube, more_than_expected_has_arrived){
  boost::mpi::communicator world;

  std::vector<uint8_t> in(15);
  std::vector<uint8_t> out(10);
  for (size_t i = 0; i < in.size(); i++) {
    in[i] = i;
  }

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  rysev_m_gypercube::GyperCube task(taskDataPar);
  ASSERT_TRUE(task.validation());
  task.pre_processing();
  ASSERT_FALSE(task.run());
  task.post_processing();
}
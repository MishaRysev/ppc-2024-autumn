// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <boost/mpi/timer.hpp>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "mpi/rysev_m_gypercube/include/ops_mpi.hpp"

TEST(rysev_m_gypercube, test_pipeline_run) {
  boost::mpi::communicator world;
  std::vector<uint8_t> in(10000);
  std::vector<uint8_t> out(10000);
  for (int i = 0; i < in.size(); i++) { in[i] = i % 256; }
  
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());
  
  auto testParTask = std::make_shared<rysev_m_gypercube::GyperCube>(taskDataPar);  
  ASSERT_TRUE(testParTask->validation());
  testParTask->pre_processing();
  testParTask->run();
  testParTask->post_processing();
  
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const boost::mpi::timer current_timer;
  perfAttr->current_timer = [&] { return current_timer.elapsed(); };
  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testParTask);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  
  for (int i = 0; i < out.size(); i++) { ASSERT_EQ(out[i], in[i]); }
  ppc::core::Perf::print_perf_statistic(perfResults);
}
TEST(rysev_m_gypercube, test_task_run) {
  boost::mpi::communicator world;
  std::vector<uint8_t> in(10000);
  std::vector<uint8_t> out(10000);
  for (int i = 0; i < in.size(); i++) { in[i] = i % 256; }
  
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());
  
  auto testParTask = std::make_shared<rysev_m_gypercube::GyperCube>(taskDataPar);  
  ASSERT_TRUE(testParTask->validation());
  testParTask->pre_processing();
  testParTask->run();
  testParTask->post_processing();
  
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const boost::mpi::timer current_timer;
  perfAttr->current_timer = [&] { return current_timer.elapsed(); };
  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testParTask);
  perfAnalyzer->task_run(perfAttr, perfResults);
  
  for (int i = 0; i < out.size(); i++) { ASSERT_EQ(out[i], in[i]); }
  ppc::core::Perf::print_perf_statistic(perfResults);
}
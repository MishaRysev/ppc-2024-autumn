// Copyright 2023 Nesterov Alexander

#include "mpi/rysev_m_gypercube/include/ops_mpi.hpp"

#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

bool rysev_m_gypercube::GyperCube::pre_processing() {
  internal_order_test();
  return true;
}

bool rysev_m_gypercube::GyperCube::validation() {
  internal_order_test();
  int size = world.size();
  if (size < 2 || (size & (size - 1)) != 0) return false;
  return true;
}

bool rysev_m_gypercube::GyperCube::run() {
  internal_order_test();
  for (int i = 0; i < std::log2(world.size()); i++) {
    int neighbr = world.rank() ^ (1 << i);
    if (neighbr < world.size()) {
      std::vector<uint8_t> to_send(taskData->inputs_count[0]), to_recv(taskData->inputs_count[0]);
      std::copy(taskData->inputs[0], taskData->inputs[0] + taskData->inputs_count[0], to_send.begin());
      if (to_send.empty()) break;
      world.send(neighbr, 0, to_send);
      world.recv(neighbr, 0, to_recv);
      if (taskData->outputs_count[0] >= to_recv.size()) {
        std::copy(to_recv.begin(), to_recv.begin() + to_recv.size(), taskData->outputs[0]);
      } else {
        return false;
      }
    }
    world.barrier();
  }
  return true;
}

bool rysev_m_gypercube::GyperCube::post_processing() {
  internal_order_test();
  return true;
}

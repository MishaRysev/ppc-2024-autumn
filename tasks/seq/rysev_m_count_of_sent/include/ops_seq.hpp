// Copyright 2023 Nesterov Alexander
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace rysev_m_count_of_sent_seq {
class SentCountSequential : public ppc::core::Task {
 public:
  explicit SentCountSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::string input_;
  int count;
};
}  // namespace rysev_m_count_of_sent_seq
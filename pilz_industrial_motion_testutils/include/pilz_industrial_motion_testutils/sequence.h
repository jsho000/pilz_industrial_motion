/*
 * Copyright (c) 2019 Pilz GmbH & Co. KG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdexcept>
#include <vector>
#include <utility>

#include <pilz_msgs/MotionSequenceRequest.h>

#include "command_types_typedef.h"
#include "motioncmd.h"

namespace pilz_industrial_motion_testutils
{

/**
 * @brief Data class storing all information regarding a Sequence command.
 */
class Sequence
{
public:
  Sequence()
  {}

public:
  /**
   * @brief Adds a command to the end of the sequence.
   * @param cmd The command which has to be added.
   */
  void add(MotionCmdUPtr cmd, const double radius = 0.);

  /**
   * @brief Returns the number of commands.
   */
  size_t size() const;

  MotionCmd& getCmd(const size_t index_cmd);
  const MotionCmd& getCmd(const size_t index_cmd) const;

  void setAllBlendRadiiToZero();
  void setBlendRadii(const size_t index_cmd, const double radius);
  double getBlendRadius(const size_t index_cmd) const;

  /**
   * @brief Deletes all commands from index 'start' to index 'end'.
   */
  void erase(const size_t start, const size_t end);

  pilz_msgs::MotionSequenceRequest toRequest() const;

private:
  std::vector<std::pair<MotionCmdUPtr, double> > cmds_;
};

inline void Sequence::add(MotionCmdUPtr cmd, const double radius)
{
  assert(cmd);
  cmds_.emplace_back( std::move(cmd), radius );
}

inline size_t Sequence::size() const
{
  return cmds_.size();
}

inline MotionCmd& Sequence::getCmd(const size_t index_cmd)
{
  return *(cmds_.at(index_cmd).first);
}

inline const MotionCmd& Sequence::getCmd(const size_t index_cmd) const
{
  return *(cmds_.at(index_cmd).first);
}

inline double Sequence::getBlendRadius(const size_t index_cmd) const
{
  return cmds_.at(index_cmd).second;
}

inline void Sequence::setBlendRadii(const size_t index_cmd, const double radius)
{
  cmds_.at(index_cmd).second = radius;
}

inline void Sequence::erase(const size_t start, const size_t end)
{
  const size_t orig_N {size()};

  if (start > orig_N || end > orig_N)
  {
    std::string msg;
    msg.append("Parameter start=").append(std::to_string(start));
    msg.append(" and end=").append(std::to_string(end));
    msg.append(" must not be greater then the number of #commands=");
    msg.append(std::to_string(size()));
    throw std::invalid_argument(msg);
  }
  cmds_.erase(cmds_.begin()+start, cmds_.begin()+end);
  if (end == orig_N)
  {
    // Make sure last radius is set zero
    cmds_.at(size()-1).second = 0.;
  }
}

}


#endif // SEQUENCE_H

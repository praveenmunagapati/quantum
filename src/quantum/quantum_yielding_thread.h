/*
** Copyright 2018 Bloomberg Finance L.P.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef QUANTUM_YIELDING_THREAD_H
#define QUANTUM_YIELDING_THREAD_H

#include <thread>
#include <chrono>
#include <quantum/quantum_util.h>
#include <quantum/quantum_thread_traits.h>

namespace Bloomberg {
namespace quantum {

//==============================================================================================
//                               struct YieldingThreadDuration
//==============================================================================================
/// @struct YieldingThreadDuration.
/// @brief This class provides the same functionality as a coroutine yield when called from a thread context.
/// @note For internal use only.
template <class DURATION>
struct YieldingThreadDuration
{
    /// @brief Yields the current thread either via a busy wait loop or by sleeping it.
    ///        Behavior is determined at compile time.
    /// @param[in] time Time used for the sleep duration.
    void operator()(DURATION time = std::chrono::duration_cast<DURATION>(ThreadTraits::yieldSleepIntervalMs()))
    {
        if (time == DURATION(0)) {
            //Busy wait
            std::this_thread::yield();
        }
        else {
            //Sleep wait
            std::this_thread::sleep_for(time);
        }
    }
};

using YieldingThread = YieldingThreadDuration<std::chrono::milliseconds>;

}}

#endif //QUANTUM_YIELDING_THREAD_H

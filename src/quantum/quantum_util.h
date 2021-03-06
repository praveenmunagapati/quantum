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
#ifndef QUANTUM_UTIL_H
#define QUANTUM_UTIL_H

#include <tuple>
#include <functional>
#include <utility>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <quantum/quantum_traits.h>
#include <quantum/interface/quantum_itask.h>
#include <quantum/interface/quantum_icontext.h>
#include <quantum/interface/quantum_ipromise.h>

namespace Bloomberg {
namespace quantum {

//fwd declarations
template <class RET> class Context;
template <class RET> class Promise;

//==============================================================================================
//                                      struct Util
//==============================================================================================
/// @struct Util.
/// @brief Utility to bind a user callable function unto a coroutine or an IO task.
/// @note For internal use only.
struct Util
{
    template<class RET, class FUNC, class ...ARGS>
    static constexpr std::function<void(Traits::Yield& yield)>
    bindCaller(std::shared_ptr<Context<RET>> ctx, FUNC&& func0, ARGS&& ...args0);
    
    template<class RET, class FUNC, class ...ARGS>
    static constexpr std::function<int()>
    bindIoCaller(std::shared_ptr<Promise<RET>> promise, FUNC&& func0, ARGS&& ...args0);
    
    //------------------------------------------------------------------------------------------
    //                                      ForEach
    //------------------------------------------------------------------------------------------
    template <class RET, class INPUT_IT>
    static int forEachCoro(CoroContextPtr<std::vector<RET>> ctx,
                           INPUT_IT inputIt,
                           size_t num,
                           const Functions::ForEachFunc<RET, INPUT_IT>& func);
    
    template <class RET, class INPUT_IT>
    static int forEachBatchCoro(CoroContextPtr<std::vector<std::vector<RET>>> ctx,
                                INPUT_IT inputIt,
                                size_t num,
                                const Functions::ForEachFunc<RET, INPUT_IT>& func,
                                size_t numCoroutineThreads);
    
    //------------------------------------------------------------------------------------------
    //                                      MapReduce
    //------------------------------------------------------------------------------------------
    template <class KEY,
              class MAPPED_TYPE,
              class REDUCED_TYPE,
              class INPUT_IT>
    static int mapReduceCoro(CoroContextPtr<std::map<KEY, REDUCED_TYPE>> ctx,
                             INPUT_IT inputIt,
                             size_t num,
                             const Functions::MapFunc<KEY, MAPPED_TYPE, INPUT_IT>& mapper,
                             const Functions::ReduceFunc<KEY, MAPPED_TYPE, REDUCED_TYPE>& reducer);
    
    template <class KEY,
              class MAPPED_TYPE,
              class REDUCED_TYPE,
              class INPUT_IT>
    static int mapReduceBatchCoro(CoroContextPtr<std::map<KEY, REDUCED_TYPE>> ctx,
                                  INPUT_IT inputIt,
                                  size_t num,
                                  const Functions::MapFunc<KEY, MAPPED_TYPE, INPUT_IT>& mapper,
                                  const Functions::ReduceFunc<KEY, MAPPED_TYPE, REDUCED_TYPE>& reducer);
    
#ifdef __QUANTUM_PRINT_DEBUG
    //Synchronize logging
    static std::mutex& LogMutex();
#endif
};

}}

#include <quantum/impl/quantum_util_impl.h>

#endif //QUANTUM_UTIL_H

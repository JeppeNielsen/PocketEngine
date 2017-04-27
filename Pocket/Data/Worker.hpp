//
//  Worker.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include <future>
#include <deque>

namespace Pocket {
    class Worker {
        private:
            struct Task {
                std::function<bool()> check;
                std::function<void()> finished;
            };
            std::deque<Task> tasks;
        public:
        void Update();
        
        template<typename Task, typename Finished>
        void DoTask(Task&& task, Finished&& finished) {
            auto future = std::async(std::launch::async, task).share();
            const static auto zeroDuration = std::chrono::seconds(0);
            tasks.push_back({
                [future] () {
                    return future.wait_for(zeroDuration) == std::future_status::ready;
                },
                [finished, future] () {
                    finished(future.get());
                }
            });
        }
    };
}

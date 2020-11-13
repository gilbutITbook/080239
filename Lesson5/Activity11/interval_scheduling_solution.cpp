#include <list>
#include <algorithm>
#include <iostream>
#include <random>

// 모든 작업은 ID와 <시작 시간, 종료 시간> 쌍으로 표현됨
struct Task
{
	unsigned ID;
	unsigned start_time;
	unsigned end_time;
};

auto initialize_tasks(int num_tasks, int max_end_time)
{
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(1, max_end_time);

	std::list<Task> tasks;

	for (unsigned i = 0; i < num_tasks; i++)
	{
		auto start_time = uniform_dist(rand);
		auto end_time = uniform_dist(rand);
		
		if (start_time == end_time) end_time++;
		if (start_time > end_time) std::swap(start_time, end_time);

		tasks.push_back({i + 1, start_time, end_time});
	}

	return tasks;
}

auto job_scheduling(std::list<Task> tasks)
{
	// 작업 종료 시간을 기준으로 정렬
	tasks.sort([](const auto& lhs, const auto& rhs) {
			return lhs.end_time < rhs.end_time;
		});

	for (auto curr_task = tasks.begin(); curr_task != tasks.end(); curr_task++)
	{
		auto next_task = std::next(curr_task, 1);

		// 현재 작업과 시간이 겹치는 작업은 제거
		while (next_task != tasks.end() &&
			next_task->start_time < curr_task->end_time)
		{
			next_task = tasks.erase(next_task);
		}
	}

	return tasks;
}

void print(std::list<Task>& tasks, int max_end_time)
{
	for (auto t : tasks) {
		std::cout << "[" << t.ID << "] " << t.start_time << " -> " << t.end_time << "\t|";

		int i = 0;
		for (; i < t.start_time; i++) std::cout << " ";
		for (; i < t.end_time; i++) std::cout << "*";
		for (; i < max_end_time; i++) std::cout << " ";
		std::cout << "|" << std::endl;
	}
}

int main()
{
	int num_tasks = 10;
	int max_end_time = 20;

	auto tasks = initialize_tasks(num_tasks, max_end_time);
	std::cout << "[전체 작업]" << std::endl;
	print(tasks, max_end_time);

	auto scheduled_tasks = job_scheduling(tasks);
	std::cout << "\n[스케쥴 조정한 작업]" << std::endl;
	print(scheduled_tasks, max_end_time);
}

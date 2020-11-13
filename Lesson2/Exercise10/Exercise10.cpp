#include <iostream>
#include <queue>
#include <vector>

struct median
{
	std::priority_queue<int> maxHeap;
	std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

	void insert(int data)
	{
		if (maxHeap.size() == 0)
		{
			maxHeap.push(data);
			return;
		}

		if (maxHeap.size() == minHeap.size())
		{
			if (data <= get())
				maxHeap.push(data);
			else
				minHeap.push(data);

			return;
		}

		if (maxHeap.size() < minHeap.size())
		{
			if (data > get())
			{
				maxHeap.push(minHeap.top());
				minHeap.pop();
				minHeap.push(data);
			}
			else
				maxHeap.push(data);

			return;
		}

		if (data < get())
		{
			minHeap.push(maxHeap.top());
			maxHeap.pop();
			maxHeap.push(data);
		}
		else
			minHeap.push(data);
	}

	double get()
	{
		if (maxHeap.size() == minHeap.size())
			return (maxHeap.top() + minHeap.top()) / 2.0;

		if (maxHeap.size() < minHeap.size())
			return minHeap.top();

		return maxHeap.top();
	}
};

int main()
{
	median med;

	med.insert(1);
	std::cout << "1 삽입 후 중앙값: " << med.get() << std::endl;

	med.insert(5);
	std::cout << "5 삽입 후 중앙값: " << med.get() << std::endl;

	med.insert(2);
	std::cout << "2 삽입 후 중앙값: " << med.get() << std::endl;

	med.insert(10);
	std::cout << "10 삽입 후 중앙값: " << med.get() << std::endl;

	med.insert(40);
	std::cout << "40 삽입 후 중앙값: " << med.get() << std::endl;
}

#include<vector>
#include<functional>
#include<cstdbool>
#include<cassert>
 
namespace zhang
{
	template <class T>
	struct Less
	{
		bool operator()(const T& x, const T& y)
		{
			return x < y;
		}
	};
 
	template <class T>
	struct Greater
	{
		bool operator()(const T& x, const T& y)
		{
			return x > y;
		}
	};
 
	template <class T, class Container = std::vector<T>, class compare = Less<T>>
	class priority_queue
	{
    private:
		Container _con;  //存储堆数据的容器
		compare _comp;   //用于实现仿函数的类对象

	public:
		priority_queue()  //默认构造函数（空堆）
		{ }
 
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: _con(first, last)
		{
            //找到最后一个非叶子结点，然后做调整
			for (int i = (_con.size() - 1 - 1) / 2; i >= 0; --i)
			{
				adjustDown(_con.size(), i);
			}
		}
 
		void push(const T& x)   //向堆中插入数据函数
		{
			_con.push_back(x);  //向容器尾部插入数据
			adjustUp(_con.size() - 1);  //执行向上调整操作
		}
 
		void pop()  //删除堆顶数据
		{
			assert(!_con.empty());
			
			std::swap(_con[0], _con[_con.size() - 1]);  //交换堆顶数据和最后面的数据
			_con.pop_back();   //删除容器最后面的数据（即：堆顶数据）
 
			adjustDown(_con.size(), 0);  //执行向下调整操作
		}
 
		size_t size()  //获取堆中数据个数
		{
			return _con.size();
		}
 
		bool empty()  //判空
		{
			return _con.empty();
		}
 
		const T& top()  //获取堆顶数据（根节点数据）
		{
			assert(!_con.empty());  
			return _con[0];
		}
 
	private:
		void adjustUp(int child)  //向上调整建堆函数
		{
			int parent = (child - 1) / 2;
			while (child > 0)
			{
				if (_comp(_con[parent], _con[child]))
				{
					std::swap(_con[parent], _con[child]);
					child = parent;
					parent = (child - 1) / 2;
				}
				else
				{
					break;
				}
			}
		}
 
		void adjustDown(int n, int parent)  //向下调整函数
		{
			int child = 2 * parent + 1;
			while (child < n)
			{
				if (child + 1 < n && _comp(_con[child], _con[child + 1]))
				{
					++child;
				}
 
				if (_comp( _con[parent], _con[child]))
				{
					std::swap(_con[parent], _con[child]);
					parent = child;
					child = 2 * parent + 1;
				}
				else
				{
					break;
				}
			}
		}
	};
}
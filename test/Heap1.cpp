#include <vector>
#include <assert.h>
#include <iostream>
#include<algorithm>
namespace kun
{
    template <class T>
    class Less
    {
    public:
        bool operator()(const T & lhs,const T & rhs)
        {
            return lhs < rhs;
        }
    };

    template <class T>
    class Greater
    {
    public:
        bool operator()(const T & lhs,const T & rhs)
        {
            return lhs > rhs;
        }
    };

    template <class T, class Container = std::vector<T>, class compare = Less<T>>
    class priority_queue
    {
    private:
        Container _con;
        compare _com;

    public:
        priority_queue() {}
        template<class InputIterator>
        priority_queue(InputIterator first,InputIterator last)
            :_con(first,last)
        {
            //最后一个非叶子结点，开始顺序向下调整,建堆
            for(int i=(_con.size()-1-1)/2;i>=0;i--)
            {
                adjustDown(_con.size(),i); //时间复杂度O(n);
            }
        }

        void push(const T &value)
        {
            // 尾插然后向上调整
            // 将数据加入容器
            _con.push_back(value);
            // 向上调整
            adjustUp(_con.size() - 1);
            return ;
        }

        void pop()
        {
            assert(!_con.empty());
            //交换第一个元素和最后一个元素
            std::swap(_con[0],_con[_con.size()-1]);
            _con.pop_back();
            adjustDown(_con.size(),0);
            return ;
        }
        
       const T& top() const {return _con[0];}

        bool empty()
        {
            return _con.empty();
        }

        size_t size(){return _con.size();}
    private:
        void adjustUp(int child)
        {
            int parent = (child - 1) / 2;
            while (child)
            {
                //返回ture表示优先级parent小于child
                if (_com(_con[parent], _con[child]))
                {
                    std::swap(_con[child], _con[parent]);
                    child = parent;
                    parent = (child - 1) / 2;
                }
                else
                    break;
            }
            return;
        }

        void adjustDown(int n, int parent)
        {
            int child = parent*2+ 1;
            while (child<n)
            {
                if (child + 1 < n && _com(_con[child], _con[child + 1]))
                {
                    child = child + 1;
                }

                if(_com(_con[parent], _con[child]))
                {
                    std::swap(_con[child],_con[parent]);
                    parent=child;
                    child=parent*2+1;
                }
                else break;
            }
        }
    };

}

int main()
{
    std::vector<int> v={1,9,2,4,7,3,5,8};
    kun::priority_queue<int> heap(v.begin(),v.end());
    heap.push(10);
    while(!heap.empty())
    {
        std::cout<<heap.top()<<std::endl;
        heap.pop();
    }
    
    return 0;
}
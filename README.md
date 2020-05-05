# README

实现过程中参考了 https://github.com/zouxiaohang/TinySTL。



## 性能测试

### vector

   ```c++
//std::vector<int> vec;
HSTL::vector<int> vec;
ProfilerInstance::start();
int i = 0;
for (; i != 10000; ++i){
  vec.push_back(i);
}
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
   ```



| container               | quantity | time(ms) |
| ----------------------- | -------- | -------- |
| HSTL::vector&lt;int>    | 10万     | 3.53     |
| HSTL::vector&lt;int>    | 100万    | 33.69    |
| HSTL::vector&lt;int>    | 1000万   | 327.52   |
| std::vector&lt;int>     | 10万     | 6.98     |
| std::vector&lt;int>     | 100万    | 60.13    |
| std::vector&lt;int>     | 1000万   | 576.95   |
| TinySTL::vector&lt;int> | 10万     |          |
| TinySTL::vector&lt;int> | 100万    |          |
| TinySTL::vector&lt;int> | 1000万   |          |


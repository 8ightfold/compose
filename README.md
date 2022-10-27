# compose
A lambda composition utility for c++20. Made for fun, if you have any suggestions hit me up at 888ightfold@gmail.com

## Example
```cpp
auto f = Fn{ [](int i, int j, int k) { return i + j + k; } };
auto g = Fn{ [](int i, int j, int k) { return tie( i * 2, j * 2, k * 2 ); } };
auto h = Fn{ [](int i, int j, int k) { return tie( i + 1, j + 1, k + 1 ); } };

std::cout << (f + g + h)(1, 2, 3) << '\n';
```

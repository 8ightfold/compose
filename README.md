# compose
A lambda composition utility for c++20. 
To use, you must first create the initial ``compose_base`` for the expression.
This can be done by either constructing one, or by doing ``compose::S % fn``.
Once you have created your composition, you can call it like any other function.

Made for fun, if you have any suggestions hit me up at 888ightfold@gmail.com

## Example
```cpp
auto f = [](int i, int j, int k) { return i + j + k; };
auto g = [](int i, int j, int k) { return compose::tie( i * 2, j * 2, k * 2 ); };
auto h = [](int i, int j, int k) { return compose::tie( i + 1, j + 1, k + 1 ); };

std::cout << "output: " << (compose::S % f + g + h)(1, 2, 3) << '\n';
```
```
output: 18
```

## Notes
The last function in the sequence may return any value, but all the others
must return a ``compose::tuple``. All argument counts must match.
Capturing lambdas are currently unsupported.
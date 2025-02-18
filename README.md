# bitonic_sort

## Установка
```sh
git git@github.com:ChelomovS/bitonic_sort.git
cd bitonic_sort
```

Для сборки release версии:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Для сборки debug версии:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Для отладочной информации и сравнения с CPU:
```sh
cmake -B build -DANALYSE=ON -DCOMPARE_CPU=ON
```

## Использование 
```sh
cd build 
./bitonic_sort
```

## Запуск unit-тестов:
```sh
cd build/unit_tests
./unit_tests
```

## Запуск end-to-end тестов:
```sh
cd tests
./ete_tests_runner.sh 
```

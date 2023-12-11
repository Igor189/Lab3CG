## Сборка

Для зависимостей используется пакетный менеджер **[vcpkg](https://github.com/microsoft/vcpkg)**

Список зависимостей описан в `vcpkg.json`

Для сборки необходимо при конфигурации проекта указать аргумент `-DCMAKE_TOOLCHAIN_FILE=${PATH_TO_VCKPG}/scripts/buildsystems/vcpkg.cmake`

``` bash
mkdir .build
cd .build
cmake -DCMAKE_TOOLCHAIN_FILE=${PATH_TO_VCKPG}/scripts/buildsystems/vcpkg.cmake -S .. -B ./
cmake --build .
```
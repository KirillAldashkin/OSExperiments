# Эксперименты с ОС
Мои изыскания в Ассемблере и Си.
## Сборка
### Подготовка
1) Установить инструменты: NASM, QEMU и GCC. Я использую GCC [отсюда](https://github.com/lordmilko/i686-elf-tools).
2) Я собираю код с помощью [MSBuild](https://github.com/dotnet/msbuild), поэтому также нужно установить [.NET SDK](https://dotnet.microsoft.com/download)
3) Создать файл `kernel.proj.user` и установить специфичные для вашего окружения параметры, например:
```xml
<Project>
	<PropertyGroup>
		<GNUPrefix>C:/Program Files/gnu-i686-elf/bin/i686-elf-</GNUPrefix>
		<NasmPath>C:/Program Files/nasm/</NasmPath>
		<QemuPath>C:/Program Files/qemu-windbg/</QemuPath>
	</PropertyGroup>
</Project>
```
### Сборка
* `dotnet build` = создание образа системы (`build/image.bin`) и отладочных символов (`build/bin/kernel_symbols.elf`)
* `dotnet build -p:Debug=false` = создание образа системы (`build/image.bin`) с оптимизациями при компиляции
* `dotnet build -t:Run` = `dotnet build` + запуск QEMU в режиме ожидания поключения GDB.
* `dotnet build -t:Run -p:Debug=false` = `dotnet build -p:Debug=false` + запуск QEMU
* `dotnet clean` = удаление всех объектных и бинарных файлов
## Заметки
* QEMU версии `5.2.0` упорно не хотела соединяться с GDB, поэтому пришлось качать старую версию. У меня заработало на версии `2.7.50`.
* Стараюсь делать всё кроссплатформенным, но ничего не обещаю.
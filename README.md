[![Build Status](https://travis-ci.com/BorisTestov/duplicate_finder.svg?branch=master)](https://travis-ci.com/BorisTestov/duplicate_finder)

Otus cpp course project

# "Локальная поисковая система для нахождения файлов-дубликатов"
## Краткое описание

Цель проекта - реализация программы, позволяющей пользователю искать дубликаты файлов в заданных директориях. 
Программа должна иметь графический интерфейс и корректно работать на ОС Ubuntu 18.04.
Проект состоит из нескольких этапов, к каждому из которых предъявляются свои требования.
Каждый новый этап предполагает, что требования предыдущего этапа выполнены.
Результатом работы над проектом является исполняемый файл `duplicate_finder`

## 1 Этап
### Общие требования
1. Осуществлять поиск по одному или нескольким критериям из списка
  * Хеш (функция хеширования на выбор)
  * Имя
  * Размер
2. Файл не читается до конца, если он не дубликат
3. Выбор директории для поиска
4. Вывод результатов в виде списка.

## 2 этап
### Общие требования
1. Выбор нескольких директорий для поиска
2. Выбор нескольких директорий, исключающихся из поиска

## 3 этап
### Общие требования
1. Возможность сканировать только файлы, подходящие под маску/маски
2. Возможность исключать файлы, подходящие под маску/маски

## 4 этап
### Общие требования
1. Возможность удаления всех дубликатов после поиска в виде кнопки "Удалить все дубликаты"
2. Возможность выборочного удаления дубликатов (выделение файла/ов -> кнопка "Удалить")

## CI/CD

Github: https://github.com/BorisTestov/duplicate_finder <br>
Bintray: https://bintray.com/boristestov/duplicate_finder/main <br>
Travis: https://travis-ci.com/github/BorisTestov/duplicate_finder <br>
Github-pages: https://github.com/BorisTestov/duplicate_finder.github.io (https://boristestov.github.io/duplicate_finder.github.io/index.html) <br>


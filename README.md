# AvitoTestTask

## Считалка частотного словаря

Словом считается набор латинских букв, a-z A-Z. Любой другой символ считается пробелом. Регистр нужно привести.
Сортировать нужно сначала по частоте (в порядке убывания), потом по алфавиту. Вот пример.

<pre>
$ cat in.txt
The time has come, the Walrus said,
to talk of many things...
$ freq in.txt out.txt
$ cat out.txt
    2 the
    1 come
    1 has
    1 many
    1 of
    1 said
    1 talk
    1 things
    1 time
    1 to
    1 walrus
</pre>

## Результаты
<pre>
mike@gpu:~/Avito/WordCount/WordCount$ ./build.sh 
mike@gpu:~/Avito/WordCount/WordCount$ ./WordCount ../../huge.txt counts-sync.txt no
main: 10816369 micros passed
mike@gpu:~/Avito/WordCount/WordCount$ ./WordCount ../../huge.txt counts-threaded.txt yes
main: 3398127 micros passed
mike@gpu:~/Avito/WordCount/WordCount$ diff counts-sync.txt ../../out_huge.txt 
mike@gpu:~/Avito/WordCount/WordCount$ diff counts-threaded.txt ../../out_huge.txt 
</pre>

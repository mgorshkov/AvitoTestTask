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

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <conio.h> 
#include <locale.h> 
#include <time.h> 

int main() {
    FILE* fp;
    int s = -1, minindex, min, SIZE = 0, menu = 0;
    int** a; // матрица связей
    int* d; // минимальное расстояние 
    int* v; // посещенные вершины 
    int begin_index = 0;
    char name[20];
    char key;

    system("chcp 1251");
    system("cls");
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    printf("Введите название файла (до 20 символов): ");
    gets_s(name);
    fp = fopen(name, "a");

    printf("Введите количество вершин: ");
    while (SIZE <= 0) {
        scanf_s("%d", &SIZE);
        if (SIZE <= 0) {
            printf("Введено неверное значение\n");
            printf("Введите количество вершин: ");
        }
    }

    int isRandomFill = 0; // Флаг для определения способа заполнения: 0 - случайное, 1 - ручное

    printf("Выберите способ заполнения матрицы:\n0 - Случайное заполнение\n1 - Ручное заполнение\n");
    scanf_s("%d", &isRandomFill);

    int isDirected = 0; // Флаг для определения типа графа: 0 - неориентированный, 1 - ориентированный

    printf("Выберите тип графа:\n0 - Неориентированный\n1 - Ориентированный\n");
    scanf_s("%d", &isDirected);

    printf("Выберите начальную вершину (от 1 до %d): ", SIZE);
    while (begin_index < 1 || begin_index > SIZE) {
        scanf_s("%d", &begin_index);
        if (begin_index < 1 || begin_index > SIZE) {
            printf("Неверный номер вершины. Выберите от 1 до %d: ", SIZE);
        }
    }
    begin_index--; // Уменьшаем на 1 для корректной работы с индексами массивов (индексация с 0)

    a = (int**)malloc(SIZE * sizeof(int*));
    for (int i = 0; i < SIZE; i++)
        a[i] = (int*)malloc(SIZE * sizeof(int));

    if (isRandomFill == 0) {
        if (isDirected == 0) {
            // Неориентированный граф (случайное заполнение)
            for (int i = 0; i < SIZE; i++) {
                a[i][i] = 0;
                for (int j = i + 1; j < SIZE; j++) {
                    a[i][j] = rand() % 20;
                    a[j][i] = a[i][j];
                }
            }
        }
        else {
            // Ориентированный граф (случайное заполнение)
            for (int i = 0; i < SIZE; i++) {
                a[i][i] = 0;
                for (int j = 0; j < SIZE; j++) {
                    if (i != j) {
                        a[i][j] = rand() % 20;
                    }
                }
            }
        }
    }
    else {
        // Ручное заполнение (для ориентированного и неориентированного графа)
        if (isDirected == 0) {
            for (int i = 0; i < SIZE; i++) {
                a[i][i] = 0;
                for (int j = i + 1; j < SIZE; j++) {
                    printf("\nВведите расстояние %d - %d: ", i + 1, j + 1);
                    scanf_s("%d", &s);
                    if (s < 0) {
                        while (s < 0) {
                            printf("Введено неверное значение");
                            printf("\nВведите расстояние %d - %d: ", i + 1, j + 1);
                            scanf_s("%d", &s);
                        }
                    }
                    a[i][j] = s;
                    a[j][i] = s;
                }
            }
        }
        else {
            for (int i = 0; i < SIZE; i++) {
                a[i][i] = 0;
                for (int j = 0; j < SIZE; j++) {
                    printf("\nВведите расстояние от %d к %d: ", i + 1, j + 1);
                    scanf_s("%d", &s);
                    if (s < 0) {
                        while (s < 0) {
                            printf("Введено неверное значение");
                            printf("\nВведите расстояние от %d к %d: ", i + 1, j + 1);
                            scanf_s("%d", &s);
                        }
                    }
                    a[i][j] = s;
                }
            }
        }
    }

    printf("Ваша матрица:\n");
    fprintf(fp, "Ваша матрица:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%5d ", a[i][j]);
            fprintf(fp, "%5d ", a[i][j]);
        }
        printf("\n");
        fprintf(fp, "\n");
    }

    v = (int*)malloc(SIZE * sizeof(int));
    d = (int*)malloc(SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++) {
        d[i] = 10000;
        v[i] = 1;
    }
    d[begin_index] = 0;

    do {
        minindex = 10000;
        min = 10000;
        for (int i = 0; i < SIZE; i++) {
            if ((v[i] == 1) && (d[i] < min)) {
                min = d[i];
                minindex = i;
            }
        }

        if (minindex != 10000) {
            for (int i = 0; i < SIZE; i++) {
                if (a[minindex][i] > 0) {
                    s = min + a[minindex][i];
                    if (s < d[i]) {
                        d[i] = s;
                    }
                }
            }
            v[minindex] = 0;
        }
    } while (minindex < 10000);

    printf("\nКратчайшие расстояния до вершин: \n");
    fprintf(fp, "\nКратчайшие расстояния до вершин: \n");
    for (int i = 0; i < SIZE; i++) {
        printf("%5d ", d[i]);
        fprintf(fp, "%5d ", d[i]);
    }

    int* ver;
    int end = SIZE - 1;
    ver = (int*)malloc(SIZE * sizeof(int));
    ver[0] = end + 1;
    int k = 1;
    int weight = d[end];

    while (end != begin_index) {
        for (int i = 0; i < SIZE; i++) {
            if (a[i][end] != 0) {
                int s = weight - a[i][end];
                if (s == d[i]) {
                    weight = s;
                    end = i;
                    ver[k] = i + 1;
                    k++;
                }
            }
        }
    }

    printf("\nВывод кратчайшего пути до последней вершины\n");
    fprintf(fp, "\nВывод кратчайшего пути\n");
    for (int i = k - 1; i >= 0; i--) {
        printf("%3d ", ver[i]);
        fprintf(fp, "%3d ", ver[i]);
    }

    fclose(fp);
    free(a);
    free(v);
    free(d);
    free(ver);
    return 0;
}

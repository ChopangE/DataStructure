#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}


Array* CreateArray(int size){                                       //array생성
    Array *arr;
    arr = (Array*)malloc(sizeof(Array));
    arr->size = size;
    arr->values = (int*)malloc(sizeof(int)*size);
    return arr;

}
void QuickSort(Array* array, int left, int right, int depth){
    int q;
    if(left < right){                                                                                   //left<right라면 sorting
        fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, array->values[right]);        //sorting전에 정보 출력
        for(int i = 0; i< left; i++){
            fprintf(fout, "%-3s", "-");
        }
        for(int i= left; i<=right; i++){
            
            fprintf(fout, "%-3d", array->values[i]);
        }
        for(int i = right +1 ; i < array->size; i++){
            fprintf(fout, "%-3s", "-");
        }
        fprintf(fout,"\n");
        q = Partition(array,left,right,depth);                              //실질적으로 sorting은 partition에서 일어난다. 
        QuickSort(array,left, q-1,depth);                                   //left와 q-1 사이 sorting
        QuickSort(array,q+1, right,depth);                                  //q+1과 right사이 sorting
    }


}
int Partition(Array* array, int left, int right, int depth){
    int pl,pr,pivot,tmp;
    pivot = array->values[right];                                           //pivot 선정
    pl = left;                                                              //left point
    pr = right-1;                                                           //right point
    while(pl <= pr){                                                        //pl>pr 이라면 sorting완료이므로 pivot과 array->values[pl]swap
        while(array->values[pl] < pivot){pl++;}                             //pl부터 시작해서 pivot보다 큰 값을 찾으면 stop
        while(array->values[pr] > pivot){pr--;}                             //pr부터 시작해서 pivot보다 작은 값을 찾으면 stop
        if(pl <= pr){                                                       //만약 pl> pr이라면 sorting완료
            swap(&(array->values[pl]), &(array->values[pr]));               //pl과 pr swap pivot보다 작은값과 큰값의 위치가 바뀌게 된다.
            for(int i = 0; i< left; i++){                                   //swap 했으니 정보 출력
                fprintf(fout, "%-3s", "-");
            }
            for(int i= left; i<=right; i++){
                fprintf(fout, "%-3d", array->values[i]);
            }
            for(int i = right +1 ; i < array->size; i++){
                fprintf(fout, "%-3s", "-");
            }
            fprintf(fout,"\n");
            pl++;                                                           //다음 sorting을 위해 index를 한칸씩 옮긴다.
            pr--;
        }
    }
    if(pl < right){                                                         //right는 pivot의 index. pl이 right와 같으면 sorting이 필요없다. 그렇지않다면 pivot과 values[pl]값 바꿈
        swap(&(array->values[pl]), &(array->values[right]));                //swap했으니 정보출력
        for(int i = 0; i< left; i++){
            fprintf(fout, "%-3s", "-");
        }
        for(int i= left; i<=right; i++){
            fprintf(fout, "%-3d", array->values[i]);
        }
        for(int i = right +1 ; i < array->size; i++){
            fprintf(fout, "%-3s", "-");
        }
        fprintf(fout,"\n");
    }
    return pl;                                                              //pivot의 index return
}
void PrintArray(Array* array, int left, int right){                         //배열 전체 출력
    for(int i = left ; i <right ; i++){
        fprintf(fout, "%-3d",array->values[i]);
    }
    fprintf(fout, "%d\n", array->values[right]);
}
void DeleteArray(Array* array){
    free(array->values);
    free(array);

}
void swap(int* a, int* b){ 
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

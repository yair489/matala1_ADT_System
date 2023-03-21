
#include "AdptArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct AdptArray_ {
    int Arrsize;
    PElement* pelemArr;
    DEL_FUNC     del;
    COPY_FUNC   copy;
    PRINT_FUNC  print_f;
};

//Initializes an empty array (ie no members)
PAdptArray CreateAdptArray(COPY_FUNC copy1, DEL_FUNC del1,PRINT_FUNC print_f1){

    PAdptArray p= (PAdptArray)malloc(sizeof(struct AdptArray_));
    if(!p){return NULL;}

    p->pelemArr =NULL;
    p->Arrsize=0;

    p->del = del1;
    p->copy=copy1;
    p->print_f=print_f1;

    return p;
}


//Releases the object's memory (including its organs)
void DeleteAdptArray(PAdptArray p){
    int size= p->Arrsize;
    if(p->pelemArr){
        for(int i=0;i< size; i++){
            if(p->pelemArr[i]){
                (p->del)(p->pelemArr[i]);
                p->pelemArr[i]=NULL;
            }
            // free(p->pelemArr);
        }
    }
    free(p->pelemArr);
    free(p);
}

/*Gets an index and a member and saves a copy of the member
in the requested place. Releases the old member if
Exists*/
Result SetAdptArrayAt(PAdptArray p, int index, PElement organ){
    PElement curr= (p->copy)(organ);
    //If this is the first time booting
    if(!p->pelemArr){
        p->pelemArr=(PElement*)calloc((index  + 1), sizeof(PElement));
        // p->Arrsize=index+1;
        p->Arrsize=index+1;
        p->pelemArr[index]=curr;
        return SUCCESS;
    }
    //If you don't need to increase the array
    if(index<= p->Arrsize){
        (p->del)(p->pelemArr[index]);
        p->pelemArr[index]=curr;
        return SUCCESS;
    }else{//If you need to increase the array
        PElement* newpElemArr;
        if ( (newpElemArr = (PElement*)calloc((index + 1), sizeof(PElement))) == NULL){
			return FAIL;
        }
		memcpy(newpElemArr, p->pelemArr, (p->Arrsize) * sizeof(PElement));
        for(int i=0;i< p->Arrsize; i++){
            p->pelemArr[i]=NULL;
        }

		free(p->pelemArr);
		 p->pelemArr = newpElemArr;
         if(p->pelemArr[index]){
            (p->del)(p->pelemArr[index]);
         }
        p->pelemArr[index]=curr;

        p->Arrsize=index+1;
        p->pelemArr[index]=curr;
        return SUCCESS;

    }
}
/*Gets an index and returns a copy of the member
at this location*/
PElement GetAdptArrayAt(PAdptArray p, int index){
    if(index> p->Arrsize){
        return NULL;
    }
    if(p->pelemArr[index]==NULL){
        return NULL;
    }
    PElement curr= (p->copy)(p->pelemArr[index]);
    return curr;
}
/*Returns the size of the array (1-when the array is not
booted successfully)*/
int GetAdptArraySize(PAdptArray p){
    if(p->pelemArr==NULL){
        return -1;
    }
    return p->Arrsize;
}
/*Prints the elements in the array.*/
void PrintDB(PAdptArray p){
    for ( int i=0;i<p->Arrsize ; i++ ){
        if(!p->pelemArr[i]){
            // printf("null  ");
            // printf("%d : \n",i);
        }else{
            (p->print_f)(p->pelemArr[i]);//Prints only the existing members
        }
        
    }
}
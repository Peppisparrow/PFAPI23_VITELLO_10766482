#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
unsigned int dim;

typedef struct {
    int index;
    unsigned int distance;
} Node;

typedef struct {
    Node* array;
    unsigned int capacity;
    unsigned int size;
} MinHeap;


struct NodeBST{
    unsigned int stationID;
    unsigned int* car;
    int size;
    struct NodeBST* left;
    struct NodeBST* right;
};

struct NodeBST* createNode(unsigned int value) {
    struct NodeBST* node = (struct NodeBST*)malloc(sizeof(struct NodeBST));
    node->stationID = value;
    node->size=0;
    node->car = (unsigned int*)malloc(512*sizeof(unsigned int));
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct NodeBST* insertNode(struct NodeBST* root, unsigned int value, unsigned int numCars) {
    struct NodeBST* ph=NULL;
    struct NodeBST* node=root;
    while (node != NULL) {
        ph=node;
        if(value < node->stationID)
            node=node->left;
        else if(value > node->stationID)
            node=node->right;
        else{
            return NULL;
        }
    }
    struct NodeBST* new=createNode(value);
    for (unsigned int i = 0; i < numCars; i++) {
        char s3[32];
        unsigned int autonomy;
        if (scanf("%s", s3) == EOF)return NULL;
        autonomy = strtol(s3,NULL,10);
        new->car[new->size]=autonomy;
        new->size++;
    }
    dim++;
    if(ph==NULL) root=new;
    else if(value < ph->stationID)
        ph->left=new;
    else
        ph->right=new;
    return root;
}
struct NodeBST* minValueNode(struct NodeBST* node) {
    struct NodeBST* current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

unsigned int maxValueCar(unsigned int car[],int size) {
    unsigned int max=0;
    for (unsigned int i = 0; i < size; i++) {
        if(car[i]>max)max=car[i];
    }
    return max;
}

void inorderTraversal(struct NodeBST* root, unsigned int* stations,unsigned int* cars,unsigned  int* index,unsigned int start,unsigned int end) {
    if (root == NULL) {
        return;
    }
    if(root->stationID>start)inorderTraversal(root->left, stations,cars, index, start, end);
    if(root->stationID>=start && root->stationID<=end){
        stations[*index] = root->stationID;
        cars[*index] = maxValueCar(root->car,root->size);
        *index = *index + 1;
    }
    if(root->stationID<end)inorderTraversal(root->right, stations,cars, index,start, end);
}

struct NodeBST* searchNode(struct NodeBST* root,unsigned int value) {
    struct NodeBST* node=root;
    while (node != NULL) {
        if(value < node->stationID)
            node=node->left;
        else if(value > node->stationID)
            node=node->right;
        else{
            return node;
        }
    }
    return NULL;
}

struct NodeBST* deleteNode(struct NodeBST* root, unsigned int value) {
    if (root == NULL){
        printf("non demolita\n");
        return root;
    }
    if (value < root->stationID)
        root->left = deleteNode(root->left, value);
    else if (value > root->stationID)
        root->right = deleteNode(root->right, value);
    else {

        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
            printf("demolita\n");
        }

        else if (root->left == NULL) {
            struct NodeBST* temp = root;
            root = root->right;
            free(temp);
            printf("demolita\n");
        } else if (root->right == NULL) {
            struct NodeBST* temp = root;
            root = root->left;
            free(temp);
            printf("demolita\n");
        }

        else {
            struct NodeBST* temp = minValueNode(root->right);

            root->stationID = temp->stationID;
            root->size = temp->size;
            for (unsigned int i = 0; i < temp->size; i++) {
                root->car[i] = temp->car[i];
            }

            root->right = deleteNode(root->right, temp->stationID);
        }
    }

    return root;
}

void deleteCar(struct NodeBST* element, unsigned int value) {
    for(int i = 0; i < element->size; i++) {
        if(element->car[i] == value) {
            for(int j = i; j < element->size - 1; j++) {
                element->car[j] = element->car[j + 1];
            }
            element->size--;
            printf("rottamata\n");
            return;
        }
    }
    printf("non rottamata\n");

}
MinHeap createMinHeap(unsigned int capacity) {
    MinHeap minHeap;
    minHeap.capacity = capacity;
    minHeap.size = 0;
    minHeap.array = malloc(capacity * sizeof(Node));
    return minHeap;
}

void swapNodes(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap,unsigned int idx,unsigned int* distances) {
    unsigned  int smallest = idx;
    unsigned int leftChild = 2 * idx + 1;
    unsigned int rightChild = 2 * idx + 2;

    if (leftChild < minHeap->size && distances[minHeap->array[leftChild].index] < distances[minHeap->array[smallest].index])
        smallest = leftChild;

    if (rightChild < minHeap->size && distances[minHeap->array[rightChild].index] < distances[minHeap->array[smallest].index])
        smallest = rightChild;

    if (smallest != idx) {
        swapNodes(&minHeap->array[idx], &minHeap->array[smallest]);
        minHeapify(minHeap, smallest, distances);
    }
}

unsigned int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

Node extractMin(MinHeap* minHeap,unsigned int* distances) {
    Node minNode = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0, distances);
    return minNode;
}

void decreaseDistance(MinHeap* minHeap, int index,unsigned  int distance,unsigned  int* distances) {
    int i;
    for (i = 0; i < minHeap->size; i++) {
        if (minHeap->array[i].index == index) {
            break;
        }
    }

    distances[index] = distance;

    while (i && distances[minHeap->array[(i - 1) / 2].index] > distances[minHeap->array[i].index]) {
        swapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

unsigned int* getShortestPath(unsigned int* prev, unsigned  int start, unsigned int end,unsigned  int* pathSize) {
    unsigned int* path = NULL;
    unsigned  int size = 0;
    unsigned int current = end;
    while (current != start) {
        if(current==UINT_MAX){
            *pathSize=0;
            return NULL;
        }
        size++;
        current = prev[current];
    }
    size++;
    current = end;
    path = malloc(size * sizeof(unsigned int));
    for(int i=0;i<size;i++){
        path[i]=current;
        current=prev[current];
    }
    *pathSize = size;
    return path;
}
unsigned int* dijkstrawithbst(struct NodeBST* root, unsigned int* pathSize,unsigned int start,unsigned int end) {
    unsigned int* path=NULL;
    if(start==end){
        *pathSize=1;
        path=(unsigned int*) malloc(sizeof(unsigned int));
        path[0]=start;
        return path;
    }
    unsigned int* stations= (unsigned int*) malloc(dim * sizeof(unsigned int));
    unsigned int* cars= (unsigned int*) malloc(dim * sizeof(unsigned int));
    unsigned int dimArray=0;
    inorderTraversal(root,stations,cars,&dimArray,start,end);

    unsigned int* distances = malloc(dimArray * sizeof(unsigned int));
    unsigned int* prev = malloc(dimArray * sizeof(unsigned int));
    unsigned int* visited = malloc(dimArray * sizeof(unsigned int));
    unsigned int range;
    for (unsigned int i = 0; i < dimArray; i++) {
        distances[i] = UINT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    distances[0] = 0;

    MinHeap minHeap = createMinHeap(dimArray);
    minHeap.size = dimArray;

    for (int i = 0; i < dimArray; i++) {
        minHeap.array[i].index = i;
        minHeap.array[i].distance = distances[i];
    }

    while (!isEmpty(&minHeap)) {
        Node minNode = extractMin(&minHeap, distances);
        int min_index = minNode.index;

        visited[min_index] = 1;

        range=cars[min_index]+stations[min_index];
        if(UINT_MAX-stations[min_index]<=cars[min_index])range=UINT_MAX;
        int j=min_index+1;
        while(stations[j]<=range){
            if(j<dimArray){
                if (visited[j] == 0 && distances[min_index] + j-min_index+1 < distances[j]) {
                    distances[j] = distances[min_index] + j-min_index+1;
                    prev[j] = min_index;
                    decreaseDistance(&minHeap, j, distances[j], distances);
                }
                j++;
            }
            else break;
        }
    }

    if(distances[dimArray-1]==UINT_MAX){
        *pathSize=0;
        return NULL;
    }
    else path = getShortestPath(prev, 0, dimArray-1, pathSize);
    for(int i=0;i<*pathSize;i++){
        path[i]=stations[path[i]];
    }
    free(stations);
    free(cars);
    free(distances);
    free(prev);
    free(visited);
    free(minHeap.array);

    return path;
}
unsigned int* dijkstraReverseWithBST(struct NodeBST* root, unsigned int* pathSize, unsigned int start, unsigned int end) {
    if(start<end) return dijkstrawithbst(root,pathSize,start,end);
    unsigned int* path = NULL;

    if (start == end) {
        *pathSize = 1;
        path = (unsigned int*)malloc(sizeof(unsigned int));
        path[0] = start;
        return path;
    }

    unsigned int* stations = (unsigned int*)malloc(dim * sizeof(unsigned int));
    unsigned int* cars = (unsigned int*)malloc(dim * sizeof(unsigned int));
    unsigned int dimArray = 0;

    inorderTraversal(root, stations, cars, &dimArray, end, start);

    unsigned int* distances = (unsigned int*)malloc(dimArray * sizeof(unsigned int));
    unsigned int* prev = (unsigned int*)malloc(dimArray * sizeof(unsigned int));
    unsigned int* visited = (unsigned int*)malloc(dimArray * sizeof(unsigned int));
    unsigned int range;

    for (unsigned int i = 0; i < dimArray; i++) {
        distances[i] = UINT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    distances[dimArray - 1] = 0;

    MinHeap minHeap = createMinHeap(dimArray);
    minHeap.size = dimArray;

    minHeap.array[0].index = (int)dimArray - 1;
    minHeap.array[0].distance = 0;

    for (int i = 1; i < dimArray; i++) {
        minHeap.array[i].index = i-1;
        minHeap.array[i].distance = distances[i];
    }

    while (!isEmpty(&minHeap)) {
        Node minNode = extractMin(&minHeap, distances);
        int min_index = minNode.index;

        visited[min_index] = 1;

        range =stations[min_index] - cars[min_index];
        if(stations[min_index]<cars[min_index]) range=0;
        int j = min_index - 1;

        while (stations[j] >= range) {
            if (j >= 0) {
                if (visited[j] == 0 && distances[min_index] + 1 < distances[j]) {
                    distances[j] = distances[min_index] + 1;
                    prev[j] = min_index;
                    decreaseDistance(&minHeap, j, distances[j], distances);
                }
                else if (visited[j] == 0 && distances[min_index] + 1 == distances[j]) {
                    if(min_index<prev[j])prev[j] = min_index;
                }
                j--;
            } else {
                break;
            }
        }
    }

    if (distances[0] == UINT_MAX) {
        *pathSize = 0;
        return NULL;
    } else {
        path = getShortestPath(prev, dimArray - 1, 0, pathSize);
    }

    for (int i = 0; i < *pathSize; i++) {
        path[i] = stations[path[i]];
    }

    free(stations);
    free(cars);
    free(distances);
    free(prev);
    free(visited);
    free(minHeap.array);

    return path;
}
int main() {
    dim=0;
    struct NodeBST* root = NULL;
    struct NodeBST* tempStation= NULL;
    char c;
    int index;
    char command[32];
    char s1[32];
    char s2[32];
    char s3[32];
    unsigned int distance, numCars, autonomy;

    while ((c=getc_unlocked(stdin)) != EOF) {
        index=0;
        if(c != '\n') command[index++]=c;
        while((c=getc_unlocked(stdin)) != ' ') {
            if (c == EOF)return 0;
            command[index++] = c;
        }
        if (strncmp(command, "rottama-auto",12) == 0) {
            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            autonomy = strtol(s3,NULL,10);
            tempStation=searchNode(root,distance);
            if(tempStation==NULL)printf("non rottamata\n");
            else{
                deleteCar(tempStation, autonomy);
            }
        }

        else if (strncmp(command, "aggiungi-auto",13) == 0) {

            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            autonomy = strtol(s3,NULL,10);
            tempStation=searchNode(root,distance);
            if(tempStation==NULL)printf("non aggiunta\n");
            else{
                tempStation->car[tempStation->size]=autonomy;
                tempStation->size++;
                printf("aggiunta\n");
            }
        }


        else if (strncmp(command, "aggiungi-stazione",17) == 0) {

            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s2) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            numCars = strtol(s2,NULL,10);
            tempStation=insertNode(root, distance, numCars);
            if(tempStation==NULL){
                printf("non aggiunta\n");
                for(int i=0;i<numCars;i++) if(scanf("%s",s1)==EOF)return 0;
            }
            else{
                root=tempStation;
                printf("aggiunta\n");
            }

        }

        else if (strncmp(command, "demolisci-stazione",18) == 0) {

            if(scanf("%s", s1) == EOF)return 0;
            distance = strtol(s1,NULL,10);

            tempStation=deleteNode(root, distance);
            root=tempStation;


        }
        else if (strncmp(command, "pianifica-percorso",18) == 0) {

            unsigned int startDistance, endDistance;
            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            startDistance = strtol(s1,NULL,10);
            endDistance = strtol(s3,NULL,10);
            unsigned int pathSize;
            unsigned int* shortestPath = dijkstraReverseWithBST(root, &pathSize, startDistance, endDistance);
            if (pathSize == 0) {
                printf("nessun percorso\n");
            }
            else{
                for (; pathSize > 0; pathSize--) {
                    printf("%u", shortestPath[pathSize-1]);
                    if((pathSize-1)!=0)printf(" ");
                }
                printf("\n");
                free(shortestPath);
            }
        }
    }
    return 0;
}




#include <iostream>

class Heap{
private:
    const int MinData = -1;
    int *Elements;
    int Capacity;
    int Size;
public:
    Heap(int MaxElements){
        Elements = new int [MaxElements+1];
        Capacity = MaxElements;
        Size=0;
        Elements[0]=MinData;
    }
    Heap(int n,int a[],int MaxElements){
        Elements = new int [MaxElements+1];
        Capacity = MaxElements;
        Size=n;
        Elements[0]=MinData;
        int i;
        for(i=1;i<=n;i++) Elements[i]=a[i-1];
        for(i=n/2;i>0;i--) PercolateDown(i);
    }
    ~Heap(){
        delete Elements;
    }
    void PercolateUp(int p){
        int x = Elements[p];
        int i;
        for(i=p;x<=Elements[i/2];i/=2){
            Elements[i] = Elements[i/2];
        }
        Elements[i] = x;
    }
    void PercolateDown(int p){
        int x = Elements[p];
        int i,child;
        for(i=p;i*2<=Size;i=child){
            child=i*2;
            if(child!=Size && Elements[child+1]<Elements[child]){
                child++;
            }
            if(x>= Elements[child]){
                Elements[i]=Elements[child];
            }else{
                break;
            }
        }
        Elements[i] = x;
    }
    void Insert(int X) 
    {
        int p = ++Size;
        Elements[p] = X;
        PercolateUp(p);
    }
    int DeleteMin(void) 
    { 
        int MinElement; 
        MinElement = Elements[1];
        Elements[1] = Elements[Size--];
        PercolateDown(1);
        return MinElement; 
    }
    void Print(void){
        for(int i=1;i<=Size;i++){
            std::cout << Elements[i] << " ";
        }
    }
};

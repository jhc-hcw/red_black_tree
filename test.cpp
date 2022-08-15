#include"rb_tree.h"

const int countj=500000;//设置测试数据量
#include<random>
int main(){
    rb_tree<int> tr;
    random_device  seed;
    ranlux48 engine(seed());
    uniform_int_distribution<> distru(0,INT32_MAX);
    int *array=new int[countj];
    //int arrays[10]={15,28,21,24,44,92,55,84,8,9};
    for(int i=0;i<countj;i++){
        int temp=distru(engine);  //生成随机树
        array[i]=temp;
        //cout<<temp<<",";
        tr.emplace_node(array[i]);
    }
    for(int i=0;i<countj;i++){
        tr.erase_data(array[i]);
    }
    cout<<endl;
    tr.layer_order(); //按层遍历树
    delete[] array;
    return 9;
}

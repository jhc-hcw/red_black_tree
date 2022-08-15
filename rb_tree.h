#include<iostream>
using namespace std;
#include"myutil.h"

template <typename T>
struct tree;
template<typename T>
using tree_ptr=tree<T> *;
template<typename T>
struct rb_tree_t;
template<typename T>
using rb_tree_ptr=rb_tree_t<T> *;

/*
template<typename T>
struct tree{
    using type=T;
    T data;
    tree_ptr<T> father,lchild,rchild;
    tree(T tree_data={},tree_ptr<T> father_v=nullptr,tree_ptr<T> lchild_v=nullptr,tree_ptr<T> rchlid_v=nullptr ):data(tree_data),
    father(father_v),lchild(lchild_v),rchild(rchlid_v){};
    ~tree(){
        delete lchild;
        delete rchild;
    }
    //执行这个方法，删除树不会递归删除它的子树；
    void clear_t();
};
template <typename T>
void tree<T>::clear_t(){
    lchild=nullptr;
    rchild=nullptr;
}
*/    //废弃，发现用继承体系父子节点获取红黑属性要转型，太蠢了
enum class rb_tree_color{
    red=1,black=2
};
enum class rb_tree_type{
    red,
    black,
    head,
    father_l,
    father_r,
    red_tri_,
    red_tri_l,
    red_tri_r,
    red_four_,
    red_four_r,
    red_four_l,
    black_dou,
    black_four,
    black_tri_,
    black_tri_lred,
    black_tri_rred
};
template <typename T>
struct rb_tree_t{
    T data;
    rb_tree_ptr<T> father,lchild,rchild;
    rb_tree_color tree_color;
    rb_tree_t(T &&tree_data={},rb_tree_color t=rb_tree_color::red,rb_tree_ptr<T> father_v=nullptr,rb_tree_ptr<T> lchild_v=nullptr,rb_tree_ptr<T> rchlid_v=nullptr):
            data(std::forward<T>(tree_data)),father(father_v),lchild(lchild_v),rchild(rchlid_v) ,tree_color(t){};
    //递归删除节点和所有子节点
    ~rb_tree_t(){
        delete lchild;
        delete rchild;
    }
    //复制构造应该删除，不然会产生循环递归的情况。
    rb_tree_t(rb_tree_t const & )=delete;
    rb_tree_t(rb_tree_t const * )=delete;
    rb_tree_t(rb_tree_t &&)=delete;
    bool type_is(rb_tree_type t)const;
    //执行这个方法，删除树不会递归删除它的子树；
    void clear_t();
};

//树节点内置判断是否某种情况的方法，所有情况都写了，
// 这样写的好处是在需要判断是不是哪种类型的时候直接返回真假进if判断了，不需要再写逻辑。我觉得非常香。
template<typename T>
bool rb_tree_t<T>::type_is(rb_tree_type t)const {
    switch(t){
        case rb_tree_type::red :
            if(this->tree_color==rb_tree_color::red){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black :
            if(this->tree_color==rb_tree_color::black){
                return true;
            }else{
                break;
            }
        case rb_tree_type::head:
            if(father== nullptr){
                return true;
            }else{
                break;
            }
        case rb_tree_type::father_l:
            if(father && father->lchild==this){
                return true;
            }else{
                break;
            }
        case rb_tree_type::father_r:
            if(father && father->rchild==this){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black_dou:
            if(type_is(rb_tree_type::black) && (!rchild|| rchild->tree_color==rb_tree_color::black) &&(!lchild ||lchild->tree_color==rb_tree_color::black)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black_four:
            if(type_is(rb_tree_type::black) && (lchild &&lchild->tree_color==rb_tree_color::red) && (rchild && rchild->tree_color==rb_tree_color::red)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black_tri_lred:
            if(type_is(rb_tree_type::black) && (lchild && lchild->tree_color==rb_tree_color::red) &&(!rchild || rchild->tree_color==rb_tree_color::black)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black_tri_rred:
            if(type_is(rb_tree_type::black) && (rchild && rchild->tree_color==rb_tree_color::red) && (!lchild || lchild->tree_color==rb_tree_color::black)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::black_tri_:
            if(type_is(rb_tree_type::black_tri_rred) || type_is(rb_tree_type::black_tri_lred)){
                return true;
            }else {
                break;
            }
        case rb_tree_type::red_tri_l:
            if(type_is(rb_tree_type::red)&&father && father->type_is(rb_tree_type::black_tri_lred)){
                return true;
            }
        case rb_tree_type::red_tri_r:
            if(type_is(rb_tree_type::red) && father && father->type_is(rb_tree_type::black_tri_rred)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::red_four_:
            if(type_is(rb_tree_type::red) && father && father->type_is(rb_tree_type::black_four)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::red_four_r:
            if(type_is(rb_tree_type::red_four_) &&type_is(rb_tree_type::father_r)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::red_four_l:
            if(type_is(rb_tree_type::red_four_) && type_is(rb_tree_type::father_l)){
                return true;
            }else{
                break;
            }
        case rb_tree_type::red_tri_:
            if(type_is(rb_tree_type::red)&&father && father->type_is(rb_tree_type::black_tri_)){
                return true;
            }else{
                break;
            }
    }
    return false;
}
template <typename T>
void rb_tree_t<T>::clear_t(){
    lchild=nullptr;
    rchild=nullptr;
}
//对树节点的封装类。可有可无。
template<typename T>
class rb_tree{
public:
    rb_tree_ptr<T> root;
    void insert_blance(rb_tree_ptr<T> p);
    rb_tree():root(nullptr){};
    rb_tree(const rb_tree &)=delete;
    rb_tree(const rb_tree &&)=delete;
    //通过节点插入其中
    bool insert_node(rb_tree_ptr<T> p);
    //给定值直接置入树中，建议使用这个方法。
    bool emplace_node(T data);
    //删除值对值的节点
    bool erase_data(T target) ;
    //删除对应节点
    void erase_node(rb_tree_ptr<T> p) ;
    ~rb_tree(){
        delete root;
    }
    int layer_order()const;
private:
    void delete_dir(rb_tree_ptr<T> p);
    void change_struct(rb_tree_ptr<T> p);
    //寻找目标值
    pair<rb_tree_ptr<T>,rb_tree_ptr<T>> find_node(T &&target)const;
};
template<typename T>
pair<rb_tree_ptr<T>,rb_tree_ptr<T>> rb_tree<T>::find_node(T &&target) const {
    auto p=root;
    rb_tree_ptr<T> f= nullptr;
    while(p){
        if(p->data==target){
            return pair<rb_tree_ptr<T>,rb_tree_ptr<T>>(f,p);
        }else if(p->data>target){
            f=p;
            p=p->lchild;
        }else if(p->data<target){
            f=p;
            p=p->rchild;
        }
    }
    return pair<rb_tree_ptr<T>,rb_tree_ptr<T>>(f, p);
}
template<typename T>
rb_tree_ptr<T> r_rotate(rb_tree_ptr<T> tree){
    auto l=tree->lchild;
    if(tree && l){
        if(tree->father){
            if(tree->type_is(rb_tree_type::father_l)){
                tree->father->lchild=l;
            }else{
                tree->father->rchild=l;
            }
        }
        l->father=tree->father;
        tree->father=l;
        tree->lchild=l->rchild;
        l->rchild=tree;
        if(tree->lchild){
            tree->lchild->father=tree;
        }
    }else{
        return nullptr;
    }
    return l;
}
template<typename T>
rb_tree_ptr<T> l_rotate(rb_tree_ptr<T> tree){
    auto r=tree->rchild;
    if(tree && r){
        if(tree->father){
            if(tree->type_is(rb_tree_type::father_l)){
                tree->father->lchild=r;
            }else{
                tree->father->rchild=r;
            }
        }
        r->father=tree->father;
        tree->father=r;
        tree->rchild=r->lchild;
        r->lchild=tree;
        if(tree->rchild){
            tree->rchild->father=tree;
        }
    }else{
        return nullptr;
    }
    return r;
}
template<typename T>
bool rb_tree<T>::insert_node(rb_tree_ptr<T> p){
    if(p->lchild|| p->rchild){
        cout<<"error"<<endl;
        return false;
    }
    p->tree_color=rb_tree_color::red;
    auto res= find_node(std::forward<T>(p->data));
    if(res.second){
        return false;
    }else if(!res.first){
        root=p;
        root->tree_color=rb_tree_color::black;
        return true;
    }
    if(p->data>res.first->data){
        res.first->rchild=p;
        p->father=res.first;
    }else{
        res.first->lchild=p;
        p->father=res.first;
    }
    if(res.first->type_is(rb_tree_type::red)){
    }
    if(res.first->tree_color==rb_tree_color::red){
        insert_blance(p);
    }
    return true;
};
template<typename T>
void rb_tree<T>::insert_blance(rb_tree_ptr<T> p) {
    auto rf=p->father;
    auto bff=rf->father;
    if(rf->type_is(rb_tree_type::red_tri_)){
        if(rf->type_is(rb_tree_type::father_r) && p->type_is(rb_tree_type::father_r)){
            rf->tree_color=rb_tree_color::black;
            bff->tree_color=rb_tree_color::red;
            l_rotate(bff);
            if(bff==root){
                root=rf;
            }
        }else if(rf->type_is(rb_tree_type::father_r) && p->type_is(rb_tree_type::father_l)){
            r_rotate(rf);
            bff->tree_color=rb_tree_color::red;
            p->tree_color=rb_tree_color::black;
            l_rotate(bff);
            if(bff==root){
                root=p;
            }
        }else if(rf->type_is(rb_tree_type::father_l)&& p->type_is(rb_tree_type::father_l)){
            rf->tree_color=rb_tree_color::black;
            bff->tree_color=rb_tree_color::red;
            r_rotate(bff);
            if(bff==root){
                root=rf;
            }
        }else {
            l_rotate(rf);
            bff->tree_color=rb_tree_color::red;
            p->tree_color=rb_tree_color::black;
            r_rotate(bff);
            if(bff==root){
                root=p;
            }
        }
    }else{
        if(bff==root){
           root->lchild->tree_color=rb_tree_color::black;
           root->rchild->tree_color=rb_tree_color::black;
        }else{
//            cout<<root<<"   |   "<<bff<<endl;
//            cout<<root->data<<"  "<<bff->data<<endl;
//            return ;
            bff->lchild->tree_color=rb_tree_color::black;
            bff->rchild->tree_color=rb_tree_color::black;
            bff->tree_color=rb_tree_color::red;
            if(bff->father->type_is(rb_tree_type::red))
                insert_blance(bff);
        }
    }
}
template<typename T>
bool rb_tree<T>::emplace_node(T data){
    auto temp=new rb_tree_t<T>(std::forward<T>(data),rb_tree_color::red);
    return insert_node(temp);
};

template<typename T>
int rb_tree<T>::layer_order()const{
    if(!root){
        cout<<"空树"<<endl;
        return 0;
    }
    jhc_util::stack<rb_tree_ptr<T>> *s1,*s2;
    s1=new jhc_util::stack<rb_tree_ptr<T>>;
    s2=new jhc_util::stack<rb_tree_ptr<T>>;
    auto p=root;
    int count=0;
    s1->push(p);
    cout<<s1->get_top()->data<<endl;
    while (!s1->isEmpty()) {
        while (!s1->isEmpty()) {
            if (s1->get_top()->lchild) {
                auto tp=s1->get_top()->lchild;
                s2->push(tp);
                cout << tp->data;
                if(tp->type_is(rb_tree_type::red)){
                    cout<<"r ";
                }else{
                    cout<<"b ";
                }
            }
            if (s1->get_top()->rchild) {
                auto tp=s1->get_top()->rchild;
                s2->push(tp);
                cout << tp->data;
                if(tp->type_is(rb_tree_type::red)){
                    cout<<"r ";
                }else{
                    cout<<"b ";
                }
            }
            s1->pop();
        }
        cout << endl;
        auto t = s1;
        s1 = s2;
        s2 = t;
        count++;
    }
    return count;
}
//突发奇想的方法，完全错误！！！！！！！！！！！！！
template <typename T>
void changed_color(rb_tree_ptr<T> p,rb_tree_color color){
    if(p &&p->tree_color==color ){
        auto t=color==rb_tree_color::red? rb_tree_color::black :rb_tree_color::red;
        p->tree_color=t;
        changed_color(p->lchild,t);
        changed_color(p->rchild,t);
    }
}
//改变待删节点上层树的结构，最终把节点变红删除。
template <typename T>
void rb_tree<T>::change_struct(rb_tree_ptr<T> p){
    auto father=p->father,t=p;
    while(p->tree_color==rb_tree_color::black){
        if(father && father->type_is(rb_tree_type::black_dou)){
            if(root==father){//考虑root节点另一边改变
                if(father->lchild==t){
                    auto fr=father->rchild;
                    if(fr->type_is(rb_tree_type::black_dou)){//二节点直接变色,因为是根节点，改变两边颜色不印象平衡。
                        father->lchild->tree_color=rb_tree_color ::red;
                        father->rchild->tree_color=rb_tree_color ::red;
                        father=p->father;
                        t=p;
                        continue;
                    }else if(fr->type_is(rb_tree_type::black_tri_)){ //根结点另一边是三结点的情况。   这个情况和下面的father是red，另一变是三节点比较类似，应该可以优化分支。
                        if(fr->lchild&&fr->lchild->tree_color==rb_tree_color::red){
                            fr->tree_color=rb_tree_color::red;
                            fr->lchild->tree_color=rb_tree_color::black;
                            fr=r_rotate(fr);
                            root=l_rotate(father);
                            root->rchild->tree_color=rb_tree_color ::black;
                            root->lchild->lchild->tree_color=rb_tree_color ::red;
                            father=p->father;
                            t=p;
                            continue;
                        }else{
                            root=l_rotate(father);
                            root->rchild->tree_color=rb_tree_color ::black;
                            root->lchild->lchild->tree_color=rb_tree_color ::red;
                            father=p->father;
                            t=p;
                            continue;
                        }
                    }else if(fr->type_is(rb_tree_type::black_four)){ //另一边四结点，直接变色。 ！！！！！那非根不也行 ,不对！！！，这样就不平衡了，只有根节点有权力这么做。
                        fr->tree_color=rb_tree_color::red;
                        if(fr->lchild){
                            fr->lchild->tree_color=rb_tree_color ::black;
                            fr->rchild->tree_color=rb_tree_color ::black;
                        }
                        father=p->father;
                        t=p;
                        continue;
                    }
                }else{
                    auto fl=father->lchild;
                    if(fl->type_is(rb_tree_type::black_dou)){
                        father->lchild->tree_color=rb_tree_color ::red;
                        father->rchild->tree_color=rb_tree_color ::red;
                        father=p->father;
                        t=p;
                        continue;
                    }else if(fl->type_is(rb_tree_type::black_tri_)){
                        if(fl->rchild && fl->rchild->tree_color==rb_tree_color::red){
                            fl->tree_color=rb_tree_color ::red;
                            fl->rchild->tree_color=rb_tree_color ::black;
                            fl=l_rotate(fl);
                            root=r_rotate(father);
                            root->lchild->tree_color=rb_tree_color ::black;
                            root->rchild->rchild->tree_color=rb_tree_color ::red;
                            father=p->father;
                            t=p;
                            continue;
                        }else{
                            root=r_rotate(father);
                            root->lchild->tree_color=rb_tree_color ::black;
                            root->rchild->rchild->tree_color=rb_tree_color ::red;
                            father=p->father;
                            t=p;
                            continue;
                        }
                    }else if(fl->type_is(rb_tree_type::black_four)){ //另一边四结点，直接变色。 ！！！！！那非根不也行 ,不对！！！，这样就不平衡了，只有根节点有权力这么做。
                        fl->tree_color=rb_tree_color::red;
                        if(fl->lchild){
                            fl->lchild->tree_color=rb_tree_color ::black;
                            fl->rchild->tree_color=rb_tree_color ::black;
                        }
                        father=p->father;
                        t=p;
                        continue;
                    }
                }
            }
            t=father;
            father=father->father;
            continue;
        }else if(father && father->type_is(rb_tree_type::red)){  //红色，考虑红色另一边，做决定
            if(father->lchild==t){
                auto fr=father->rchild;
                if(fr->type_is(rb_tree_type::black_dou)){  //另一边二节点，变色，再判断
                    father->tree_color=rb_tree_color ::black;
                    father->lchild->tree_color=rb_tree_color ::red;
                    father->rchild->tree_color=rb_tree_color ::red;
                    father=p->father;
                    t=p;
                    continue;
                }else if(fr->type_is(rb_tree_type::black_tri_rred)){   //另一边三结点可直接旋转
                    father->rchild->tree_color=rb_tree_color::red;
                    father->lchild->tree_color=rb_tree_color::red;
                    father->tree_color=rb_tree_color::black;
                    father->rchild->rchild->tree_color=rb_tree_color::black;
                    l_rotate(father);
                    father=p->father;
                    t=p;
                    continue;
                }else if(fr->type_is(rb_tree_type::black_tri_lred)){  //另一边三结点不可直接旋转
                    fr->tree_color=rb_tree_color ::red;
                    fr->lchild->tree_color=rb_tree_color ::black;
                    r_rotate(fr);
                    father->rchild->tree_color=rb_tree_color::red;
                    father->lchild->tree_color=rb_tree_color::red;
                    father->tree_color=rb_tree_color::black;
                    father->rchild->rchild->tree_color=rb_tree_color::black;
                    l_rotate(father);
                    father=p->father;
                    t=p;
                    continue;
                }else{ //四节点，变色，变成二节点，再考虑
                    father->tree_color=rb_tree_color::black;
                    father->lchild->tree_color=rb_tree_color::red;
                    father->rchild->tree_color=rb_tree_color::red;
                    father->rchild->rchild->tree_color=rb_tree_color::black;
                    l_rotate(father);
                    father=p->father;
                    t=p;
                    continue;
                }
            }else{
                auto fl=father->lchild;
                if(fl->type_is(rb_tree_type::black_dou)){
                    father->tree_color=rb_tree_color ::black;
                    father->lchild->tree_color=rb_tree_color ::red;
                    father->rchild->tree_color=rb_tree_color ::red;
                    father=p->father;
                    t=p;
                    continue;
                }else if(fl->type_is(rb_tree_type::black_tri_lred)){
                    father->lchild->tree_color=rb_tree_color::red;
                    father->rchild->tree_color=rb_tree_color::red;
                    father->tree_color=rb_tree_color::black;
                    father->lchild->lchild->tree_color=rb_tree_color::black;
                    r_rotate(father);
                    father=p->father;
                    t=p;
                    continue;
                }else if(fl->type_is(rb_tree_type::black_tri_rred)){
                    fl->tree_color=rb_tree_color::red;
                    fl->rchild->tree_color=rb_tree_color ::black;
                    l_rotate(fl);
                    father->lchild->tree_color=rb_tree_color::red;
                    father->rchild->tree_color=rb_tree_color::red;
                    father->tree_color=rb_tree_color::black;
                    father->lchild->lchild->tree_color=rb_tree_color::black;
                    r_rotate(father);
                    father=p->father;
                    t=p;
                }else{
                    father->tree_color=rb_tree_color::black;
                    father->rchild->tree_color=rb_tree_color::red;
                    father->lchild->tree_color=rb_tree_color::red;
                    father->lchild->lchild->tree_color=rb_tree_color::black;
                    r_rotate(father);
                    father=p->father;
                    t=p;
                    continue;
                }
            }

        }else if(father && father->type_is(rb_tree_type::black_tri_)){   //黑色，但黑色是三节点，旋转即可
            if(father->lchild==t){
                father->tree_color=rb_tree_color ::red;
                father->rchild->tree_color=rb_tree_color ::black;
                //father->rchild->rchild->tree_color=rb_tree_color::red;
                if(root==father)
                    root=l_rotate(father);
                else
                    l_rotate(father);
                father=p->father;
                t=p;
                continue;
            }else {
                father->tree_color=rb_tree_color ::red;
                father->lchild->tree_color=rb_tree_color ::black;
                //father->lchild->lchild->tree_color=rb_tree_color::red;
                if(root==father)
                    root=r_rotate(father);
                else
                    r_rotate(father);
                father=p->father;
                t=p;
                continue;
            }
        }
    }
}
//删除值对值的节点
template<typename T>
bool rb_tree<T>::erase_data(T target) {
    auto tar=find_node(std::forward<T>(target));
    if(tar.second){
        erase_node(tar.second);
        return true;
    }
    return false;
};
template<typename T>
void rb_tree<T>::delete_dir(rb_tree_ptr<T> p){
    if(p->tree_color==rb_tree_color::red){  //红色结点，直接删除
        if(p->type_is(rb_tree_type::father_l)){
            p->father->lchild= nullptr;
        }else{
            p->father->rchild=nullptr;
        }
        p->clear_t();
        delete p;
        return ;
    }else{
        if(p->father){//不是头节点
            if(p->lchild){
                p->data=move(p->lchild->data);
                p->lchild->clear_t();
                p->lchild= nullptr;
                delete p->lchild;
                return;
            }
            change_struct(p);
            delete_dir(p);
        }else{//是头节点
            if(p->rchild){
                root=p->rchild;
                root->father=nullptr;
                root->tree_color=rb_tree_color ::black;
                p->clear_t();
                delete(p);
                return ;
            }else{
                delete p;
                root=nullptr;
                cout<<"空树！"<<endl;
                return ;
            }
        }
    }
}
//删除对应节点
template<typename T>
void rb_tree<T>::erase_node(rb_tree_ptr<T> p){
    auto sub=p;
    if(sub->lchild)
        sub=sub->lchild;
    while(true){
        if(sub->rchild){
            sub=sub->rchild;
        }else{
            break;
        }
    }//找到替代节点。
    if(p!=sub)
        p->data=std::move(sub->data);
    delete_dir(sub);
} ;
/*
const int countj=5000000;
#include<random>
int main(){
    rb_tree<int> tr;
    random_device  seed;
    ranlux48 engine(seed());
    uniform_int_distribution<> distru(0,INT32_MAX);
    int *array=new int[countj];
    int arrays[10]={15,28,21,24,44,92,55,84,8,9};
    for(int i=0;i<countj;i++){
        int temp=distru(engine);
        array[i]=temp;
        //cout<<temp<<",";
        tr.emplace_node(array[i]);
    }
    for(int i=0;i<countj;i++){
        tr.erase_data(array[i]);
    }
//    tr.erase_data(arrays[0]);
//    tr.erase_data(arrays[1]);
//    tr.erase_data(arrays[2]);
//    tr.erase_data(arrays[3]);
//    tr.erase_data(arrays[4]);
//    tr.erase_data(arrays[5]);
//    tr.erase_data(arrays[6]);
//    tr.erase_data(arrays[7]);
//    tr.erase_data(arrays[8]);
//    tr.erase_data(arrays[9]);
    cout<<endl;
    tr.layer_order();
    delete[] array;
    return 9;
}
*/


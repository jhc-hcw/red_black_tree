//
// Created by jhc on 2022/5/7.
//

#ifndef RB_TREE_MYUTIL_H
#define RB_TREE_MYUTIL_H

#endif //RB_TREE_MYUTIL_H
namespace jhc_util{
#include <iostream>

    using namespace std;

    template<typename T>
    class stack {
        struct node {
            T data;
            struct node *next;
        };
        struct node *D;
    public:
        stack() {
            D = NULL;
        }

        bool isEmpty() {
            if (this->D == NULL) {
                return true;
            } else {
                return false;
            }
        }

        T get_top() {
            if (!this->isEmpty()) {
                return D->data;
            } else {
                return NULL;
            }
        }

        void push(T x) {
            node *p = new node();
            p->data = x;
            p->next = D;
            D = p;
        }

        void pop() {
            if (!this->isEmpty()) {
                node *p = D;
                D = D->next;
                delete (p);
            } else {
                return;
            }
        }
    };
    template<typename T>
    class queue{
        struct node {
            T data;
            struct node *next;
            node(T x=0):data(x),next(0){}
            ~node(){
                delete(next);
            }
        };
        node *head;
        node *rear;
    public:
        queue(){
            head=new node();
            head->data=0;
            head->next=0;
            rear=head;
        }
        void add(T x){
            rear->next=new node(x);
            rear=rear->next;
        };
        void del(){
            if(!is_null()){
                node* temp=head->next;
                head->next=temp->next;
                temp->next=NULL;
                if(temp==rear){
                    rear=head;
                }
                delete(temp);
            }
        };
        T get_rear(){
            if(!is_null()){
                return rear->data;
            }else{
                return NULL;
            }
        };
        T get_head(){
            if(!is_null()){
                return head->next->data;
            }else{
                return NULL;
            }
        };
        bool is_null(){
            if(!head->next){
                return true;
            } else{
                return false;
            }
        };
    };
    template<typename T>
    void swap(T a,T b){
        T temp=a;
        a=b;
        b=temp;
    }
}
#include<iostream>
#include<string>
#include<windows.h>
void welcome(){
    std::cout<<"欢迎使用文本查看器0.0.1"<<std::endl;
}

void menu(){
    std::cout<<"1.查看文本"<<std::endl;
    std::cout<<"2.退出"<<std::endl;
}

int main(){
    //解决乱码
    SetConsoleOutputCP(CP_UTF8);
    welcome();
    //主循环
    while(1){
        menu();
        //输入
        std::string operation;
        std::cin>>operation;

        if(operation=="1"){

        }
        else if(operation=="2"){
            return 0;
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
}
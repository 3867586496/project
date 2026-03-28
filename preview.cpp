#include<iostream>
#include<string>
#include<windows.h>
#include<fstream>
#include<limits>
void welcome(){
    std::cout<<"欢迎使用文本查看器0.0.1"<<std::endl;
}

void WelcomeMenu(){
    std::cout<<"1.查看文本"<<std::endl;
    std::cout<<"2.退出"<<std::endl;
}

void ChooseDisplayWayMenu(){
    std::cout<<"1.整个查看"<<std::endl;
    std::cout<<"2.逐行查看（每回车一次看一行）"<<std::endl;
};

std::ifstream ReadTextFile(){
    std::cout<<"请输入路径："<<std::endl;

    std::string path_str;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(std::cin,path_str);

    std::ifstream inputTextFile(path_str.c_str());
    return inputTextFile;
}

int main(){
    //解决乱码
    SetConsoleOutputCP(CP_UTF8);
    welcome();
    //主循环
    while(1){
        WelcomeMenu();
        //输入
        std::string operation;
        std::cin>>operation;

        if(operation=="1"){
            std::ifstream inputTextFile= ReadTextFile();
            while(1){
                ChooseDisplayWayMenu();

                std::string operation;
                std::cout<<"请输入查看方式："<<std::endl;
                getline(std::cin,operation);

                std::string text;
                if(operation=="1"){

                    while(getline(inputTextFile,text)){
                        std::cout<<text<<std::endl;
                    }
                    break;
                }
                else if(operation=="2"){
                    while(getline(inputTextFile,text)){
                        std::cout<<text<<std::endl;
                        std::cin.get();
                    }
                    break;
                }
                else{
                    std::cout<<"无效操作"<<std::endl;
                }
            }
            
        }
        else if(operation=="2"){
            return 0;
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
}
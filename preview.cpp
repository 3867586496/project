#include<iostream>
#include<string>
#ifdef _WIN32
#include<windows.h>
#endif
#include<fstream>
#include<thread>
#include<chrono>
#include<map>
#include<limits>
class Config{
    public:
    std::map<std::string,int> config_dict;

    void ReadConfig(std::map<std::string,int>* map){
        std::ifstream ConfigFile("datas/config.ini");
        if(!ConfigFile){
            std::cout<<"配置文件读取失败"<<std::endl;
            return;
        }
        std::string text;
        while(getline(ConfigFile,text)){
            size_t index=text.find("=");
            if(index==-1){
                continue;
            }
            (*map)[text.substr(0,index)]=std::stoi(text.substr(index+1));
        }
        ConfigFile.close();
    }

    int ReturnConfigValue(std::string key){
        return config_dict[key];
    }

    void WriteConfig(std::map<std::string,int> map){
        std::ofstream ConfigOut("datas/config.ini",std::ios::out);
        if(!ConfigOut){
            std::cout<<"配置文件写入失败"<<std::endl;
            return;
        }
        for(std::pair<std::string,int> config_pair :map){
            ConfigOut<<config_pair.first<<"="<<config_pair.second<<std::endl;
        }
        ConfigOut.close();
    }

    //集成了写入
    void ChangeValue(std::map<std::string,int> &map,std::string key,int value){
        map[key]=value;
        WriteConfig(map);
    }
    Config(){
        ReadConfig(&config_dict);
    }
};

class Menu{
    public:
    void welcome(){
        std::cout<<"欢迎使用文本查看器0.2.0"<<std::endl;
    }

    void WelcomeMenu(){
        std::cout<<"1.查看文本"<<std::endl;
        std::cout<<"2.设置"<<std::endl;
        std::cout<<"3.退出"<<std::endl;
    }

    void ChooseDisplayWayMenu(){
        std::cout<<"1.整个查看"<<std::endl;
        std::cout<<"2.逐行查看（每回车一次看一行）"<<std::endl;
        std::cout<<"3.自动播放（按时间播放每一行）"<<std::endl;
    };

    void SettingMenu(){
        std::cout<<"1.改变自动播放时间"<<std::endl;
        //std::cout<<"2."<<std::endl;
    }
};

class Log{
    
};
//传入参数是毫秒
void sleep(int milliseconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
};

void ShowColorCode(){

}

std::ifstream ReadTextFile(){
    std::cout<<"请输入路径："<<std::endl;

    std::string path_str;
    getline(std::cin,path_str);

    std::ifstream inputTextFile(path_str.c_str());
    return inputTextFile;
}

void ChooseDisplayWayFunction(Config* config,Menu* menu){
    std::ifstream inputTextFile= ReadTextFile();
    if(!inputTextFile){
        std::cout<<"文件打开失败，请检查文件路径"<<std::endl;
        return;
    }
    while(1){
        menu->ChooseDisplayWayMenu();

        std::string operation;
        std::cout<<"请输入查看方式："<<std::endl;
        getline(std::cin,operation);

        std::string text;
        if(operation=="1"){

            while(getline(inputTextFile,text)){
                std::cout<<text<<std::endl;
            }
            inputTextFile.close();
            break;
        }
        
        else if(operation=="2"){
            while(getline(inputTextFile,text)){
                std::cout<<text<<std::endl;
                std::cin.get();
            }
            inputTextFile.close();
            break;
        }
        else if(operation=="3"){
            while(getline(inputTextFile,text)){
                std::cout<<text<<std::endl;
                sleep(config->ReturnConfigValue("autoPlayTime"));
            }
            inputTextFile.close();
            break;
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
}

void SettingFunction(Config* config,Menu* menu){
    menu->SettingMenu();

    std::string operation;
    std::cout<<"请输入操作："<<std::endl;
    getline(std::cin,operation);
    if(operation=="1"){
        std::cout<<"请输入你要改变的值（单位毫秒）";
        int inputMillisecond;
        while(!(std::cin>>inputMillisecond)||inputMillisecond<=1||inputMillisecond>10000){
            std::cin.clear();
            std::cin.std::istream::ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout<<"输入范围应该为1-10000"<<std::endl;
        }
        config->ChangeValue(config->config_dict,"autoPlayTime",inputMillisecond);
        std::cin.std::istream::ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cout<<"成功！"<<std::endl;
    }
    else{
        std::cout<<"无效操作"<<std::endl;
    }
}

int main(){
    //解决乱码
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    Config* config=new Config;
    Menu* menu=new Menu;
    menu->welcome();
    //主循环
    while(1){
        menu->WelcomeMenu();
        //输入
        std::string operation;
        getline(std::cin,operation);

        if(operation=="1"){
            ChooseDisplayWayFunction(config,menu);
        }
        else if(operation=="2"){
            SettingFunction(config,menu);
        }
        else if(operation=="3"){
            return 0;
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
}
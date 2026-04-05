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
#include<vector>
#include<ctime>
#include<filesystem>
#include<typeinfo>
#define ERROR_INFO "[info]"
#define ERROR_WARNING "[warning]"
class Date{
    public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    void GetCurrentDate(int &year,int &month,int &day,int &hour,int &minute,int &second){
        time_t t=time(0)+3600*8;//为了实现北京时间
        tm* now=gmtime(&t);

        year=now->tm_year+1900;
        month=now->tm_mon+1;
        day=now->tm_mday;
        hour=now->tm_hour;
        minute=now->tm_min;
        second=now->tm_sec;
    }
    Date(){
        GetCurrentDate(year,month,day,hour,minute,second);
    }
};

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
        std::cout<<"欢迎使用文本查看器0.4.1"<<std::endl;
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
        std::cout<<"4.自动播放（按时间播放每一个字）"<<std::endl;
        std::cout<<"5.逐页查看"<<std::endl;
    };

    void SettingMenu(){
        std::cout<<"1.改变自动整行播放时间"<<std::endl;
        std::cout<<"2.改变单字播放时间"<<std::endl;
        std::cout<<"3.改变整面查看行数"<<std::endl;
    }
};

class Log{
    public:
    std::vector<std::string> Log_List;

    template<class Type> std::string GenerateConvertErrorType(Type value,std::string convertType,std::string errorType){
        std::string typeName=typeid(value).name();
        return typeName+" "+value+" convert to "+convertType+" error,the reason is "+errorType;
    }

    template<class Type> std::string GenerateConvertErrorType(Type value,std::string convertType,const char errorType){
        std::string typeName=typeid(value).name();
        return typeName+" "+value+" convert to "+convertType+" error,the reason is "+errorType;
    }

    std::string GenerateReadFileType(std::string path){
        return "Read "+path;
    }

    std::string GenerateChangeSettingType(std::string changeType,int beforeValue,int currentValue){
        return "Change "+changeType+" "+"from "+std::to_string(beforeValue)+" "+"to "+std::to_string(currentValue);
    }

    std::string GenerateLog(std::string type,std::string level,int line){
        Date* date=new Date();
        std::string Log;
        Log="["
        +std::to_string(date->year)
        +"."
        +std::to_string(date->month)
        +"."
        +std::to_string(date->day)
        +"|"
        +std::to_string(date->hour)
        +":"
        +std::to_string(date->minute)
        +":"
        +std::to_string(date->second)
        +"]"
        +"[line="
        +std::to_string(line)
        +"]"
        +level
        +type;
        delete date;
        return Log;
    }

    void AddLog(std::string log){
        std::ifstream LogIn("datas/log.txt");
        if(!LogIn){
            std::ofstream LogOut("datas/log.txt");
            LogOut.close();
        }
        LogIn.close();
        std::ofstream LogOut("datas/log.txt",std::ios::app);
        LogOut<<log<<std::endl;
        LogOut.close();
        
    }

    void ReadLog(){
        
    }
};
//传入参数是毫秒
void sleep(int milliseconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
};
//包含关闭文件
std::vector<std::string> GetAllFileText(std::ifstream& inputTextFile){
    std::vector<std::string> allText;
    std::string text;
    while(getline(inputTextFile,text)){
        allText.push_back(text);
    }
    inputTextFile.close();
    return allText;
}

void ChooseDisplayWayFunction(Config* config,Menu* menu){
    std::cout<<"请输入路径："<<std::endl;

    std::string path_str;
    getline(std::cin,path_str);

    std::ifstream inputTextFile(path_str.c_str());
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
        else if(operation=="4"){
            while(getline(inputTextFile,text)){
                for(char character:text){
                    std::cout<<character;
                    sleep(config->ReturnConfigValue("autoPlayWordTime"));
                }
                std::cout<<std::endl;
            }
            inputTextFile.close();
            break;
        }
        else if(operation=="5"){
            std::vector<std::string> allText=GetAllFileText(inputTextFile);
            unsigned int allPage;
            if(allText.size()%config->ReturnConfigValue("linePerPage")==0){
                allPage=allText.size()/config->ReturnConfigValue("linePerPage");
            }else{
                allPage=allText.size()/config->ReturnConfigValue("linePerPage")+1;
            }
            unsigned int currentPage=1;
            while(1){
                for(int i=0;i<config->ReturnConfigValue("linePerPage");i++){
                    if((currentPage-1)*config->ReturnConfigValue("linePerPage")+i>=allText.size()){
                        break;
                    }
                    std::cout<<allText[(currentPage-1)*config->ReturnConfigValue("linePerPage")+i]<<std::endl;

                }
                std::cout<<"当前处于第"<<currentPage<<"页，共有"<<allPage<<"页"<<std::endl;
                std::cout<<"请输入您要跳转的页数（输入0退出，直接换行为下一页）："<<std::endl;
                std::string inputPage;
                unsigned int turnPage=1;
                bool isSuccess=0;
                while(1){
                    std::getline(std::cin,inputPage);
                    if(inputPage==""){
                        if(currentPage==allPage){
                            std::cout<<"最后一面了"<<std::endl;
                            break;
                        }
                        currentPage++;
                        break;
                    }
                    try{
                        turnPage=std::stoi(inputPage);
                        if(turnPage<0){
                            throw std::invalid_argument("too small");
                        }
                        if(turnPage>allPage){
                            throw std::invalid_argument("too large");
                        }
                        isSuccess=1;
                    }
                    catch(std::invalid_argument& error){
                        Log* log=new Log();
                        log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputPage,"int",error.what()),ERROR_WARNING,__LINE__));
                        delete log;
                        std::cout<<"输入无效"<<std::endl;
                    }
                    catch(std::out_of_range& error){
                        Log* log=new Log();
                        log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputPage,"int","out_of_range"),ERROR_WARNING,__LINE__));
                        delete log;
                        std::cout<<"输入无效"<<std::endl;
                    }
                    // catch(const char* error){
                    //     Log* log=new Log();
                    //     log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputPage,"int",error)));
                    //     delete log;
                    //     std::cout<<"输入无效"<<std::endl;
                    // }
                    if(isSuccess){
                        currentPage=turnPage;
                        if(currentPage==0){
                            goto EXIT;
                        }
                        break;
                    }
                    
                }
                    
            }
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
    EXIT:
    std::filesystem::path absolutePath=std::filesystem::absolute(path_str.c_str());
    
    Log* log=new Log();
    log->AddLog(log->GenerateLog(log->GenerateReadFileType(absolutePath.string()),ERROR_INFO,__LINE__));
    delete log;
}

//范围为包括min和max
void ChangeIntValue(Config* config,Menu* menu,std::string valueName,int min,int max){
    int value;
    bool isSuccess=0;
    while(1){
        value=-1;
        std::string inputValue;
        std::getline(std::cin,inputValue);
        try{
            value=std::stoi(inputValue);
            if(value<min){
                throw std::invalid_argument("too small");
            }
            if(value>max){
                
                throw std::invalid_argument("too large");
            }
            isSuccess=1;
            break;
        }
        catch(std::invalid_argument error){
            Log* log=new Log();
            log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputValue,"int",error.what()),ERROR_WARNING,__LINE__));
            delete log;
            std::cout<<"输入范围应该为"<<min<<"-"<<max<<"的整数"<<std::endl;
        }
        // catch(const char* error){
        //     Log* log=new Log();
        //     log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputValue,"int",error)));
        //     delete log;
        //     std::cout<<"输入范围应该为"<<min<<"-"<<max<<"的整数"<<std::endl;
        // }
        catch(std::out_of_range){
            Log* log=new Log();
            log->AddLog(log->GenerateLog(log->GenerateConvertErrorType(inputValue,"int","out_of_range"),ERROR_WARNING,__LINE__));
            delete log;
            std::cout<<"输入范围应该为"<<min<<"-"<<max<<"的整数"<<std::endl;
        }
        
    }
    if(isSuccess){
        int oldValue=config->ReturnConfigValue(valueName);

        config->ChangeValue(config->config_dict,valueName,value);
        std::cout<<"成功！"<<std::endl;

        Log* log=new Log();
        log->AddLog(log->GenerateLog(log->GenerateChangeSettingType(valueName,oldValue,config->ReturnConfigValue(valueName)),ERROR_INFO,__LINE__));
        delete log;
    }else{
        std::cout<<"失败"<<std::endl;
    }
    
}

void SettingFunction(Config* config,Menu* menu){
    menu->SettingMenu();

    std::string operation;
    std::cout<<"请输入操作："<<std::endl;
    getline(std::cin,operation);
    if(operation=="1"){
        std::cout<<"请输入你要改变的值（单位毫秒）";
        ChangeIntValue(config,menu,"autoPlayLineTime",1,10000);
    }
    else if(operation=="2"){
        std::cout<<"请输入你要改变的值（单位毫秒）";
        ChangeIntValue(config,menu,"autoPlayCharacterTime",1,10000);
    }
    else if(operation=="3"){
        std::cout<<"请输入你要改变的值（单位面数）";
        ChangeIntValue(config,menu,"linePerPage",1,1000);
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
            delete config;
            delete menu;
            return 0;
        }
        else{
            std::cout<<"无效操作"<<std::endl;
        }
    }
}
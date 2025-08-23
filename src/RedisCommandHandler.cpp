#include "../include/RedisCommandHandler.h"
#include "../include/RedisDatabase.h"


#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
//RESP parser;

std::vector<std::string> parserRespCommand(const std::string &input){
    std::vector<std::string> tokens;
    if(input.empty()) return tokens;
    if(input[0]!='*'){
        std::istringstream iss(input);
        std::string token;
        while(iss>>token){
            tokens.push_back(token);
        }
        return tokens;
    }
    size_t pos = 0;
    if(input[pos]!='*') return tokens;
    pos++; 

    size_t crlf = input.find("\r\n",pos);
    if(crlf==std::string::npos) return tokens;
    
    int numElements = std::stoi(input.substr(pos,crlf-pos));
    pos = crlf+2;

    for(int i=0;i<numElements;i++){
        if(pos>=input.size()||input[pos]!='$') break;
        pos++;

        crlf = input.find("\r\n",pos);
        if(crlf==std::string::npos) break;
        int len = std::stoi(input.substr(pos,crlf-pos));
        pos = crlf+2;

        if(pos+len>input.size()) break;
        std::string token = input.substr(pos,len);
        tokens.push_back(token);
        pos+=len+2;

    }
    return tokens;
}


RedisCommandHandler::RedisCommandHandler(){};

std::string RedisCommandHandler::processCommand(const std::string& commandLine){
    auto tokens = parserRespCommand(commandLine);
    if(tokens.empty()) return "-Error: Empty command\r\n";

    // std::cout<<commandLine<<"\n";

    // for(auto&t:tokens){
    //     std::cout<<t<<"\n";
    // }

    std::string cmd = tokens[0];
    std::transform(cmd.begin(),cmd.end(),cmd.begin(),::toupper);
    std::ostringstream response;
    RedisDatabase& db = RedisDatabase::getInstance();

    //common commands
    if(cmd=="PING"){
        response<<"+PONG\r\n";
    }
    else if(cmd=="ECHO"){
        if(tokens.size()<2){
            response<<"-Error: ECHO rerquires a message\r\n";
        }
        else{
            response<<"+"<<tokens[1]<<"\r\n";
        }
    }
    else if(cmd=="FLUSHALL"){
        db.flushAll();
        response<<"+OK\r\n";
    }
    else if(cmd=="SET"){
        if(tokens.size()<3){
            response<<"-Error: SET requires key and values\r\n";
        }
        else{
            db.set(tokens[1],tokens[2]);
            response<<"+OK\r\n";
        }
    }
    else if(cmd=="GET"){
        if (tokens.size() < 2){
            response << "-Error: GET requires key\r\n";
        }
        else{
            std::string value;
            if(db.get(tokens[1],value)){
                response<<"$"<<value.size()<<"\r\n"<<value<<"\r\n";
            }
            else{
                response<<"$-1\r\n";
            }
        }
    }
    else if(cmd=="KEYS"){
        std::vector<std::string> allKeys = db.keys();
        response <<"*"<<allKeys.size()<<"\r\n";
        for(const auto&it:allKeys){
            response<<"$"<<it.size()<<"\r\n"<<it<<"\r\n";
        }
    }
    else if(cmd=="TYPE"){
        if(tokens.size()<2){
            response<<"-Error: TYPE requires key\r\n";
        }
        else{
            response<<"+"<<db.type(tokens[1])<<"\r\n";
        }
    }
    else if(cmd=="DEL"||cmd=="UNLINK"){
        if(tokens.size()<2)
            response<<"-Error: "<<cmd<< " requires key\r\n";
        else{
            bool res = db.del(tokens[1]);
            response<<"+"<<(res?1:0)<<"\r\n";
        }
    }
    else if(cmd=="EXPIRE"){
        if (tokens.size() < 3)
            return "-Error: EXPIRE requires key and time in seconds\r\n";
        try{
            int seconds = std::stoi(tokens[2]);
            if (db.expire(tokens[1], seconds))
                return "+OK\r\n";
            else
                return "-Error: Key not found\r\n";
        }
        catch (const std::exception &){
            return "-Error: Invalid expiration time\r\n";
        }
    }
    else if(cmd=="RENAME"){
        if(tokens.size()<3){
            response << "-Error: RENAME requires old key and new key\r\n";
        }
        else{
            db.rename(tokens[1],tokens[2]);
            response<<"+OK\r\n";
        }
    }
    else{
        response<<"-Error:Unkown command\r\n";
    }

    return response.str();
}
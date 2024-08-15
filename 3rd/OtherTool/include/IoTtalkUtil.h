#ifndef _IoTtalkUtil_H
#define _IoTtalkUtil_H_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <curl/curl.h>
#include <chrono>
#include <json.hpp>
#include "RequestUtil.h"

using namespace std;
using namespace nlohmann;

string iottalk_server = "140.114.77.93" ;
string iottalk_server_port = "9999";
string default_input_feature = "ObjectDetect-I"; // 建立的 model input device feature
string default_output_feature = "Image-O"; // 建立的 model output device feature
string default_mac = "AIModelImage"; //要註冊的
string default_device_name = "AIModelImage";

int max_retry = 20;

class iottalk_helper{
    public:
    string device_name = "AIModelImage";
    string mac_addr = default_mac;
    vector<string> feature_list = {default_input_feature, default_output_feature};

    iottalk_helper(string device_name = default_device_name ,vector<string> feature_list = {}, string mac_addr = default_mac){
        this->device_name = device_name;
        for(auto f : feature_list)
            this->feature_list.emplace_back(f);
        this->mac_addr = mac_addr;
    }

    string Register(){
        json params = {{
            "profile", {
                {"d_name", device_name}, // 註冊device名稱
                {"dm_name", "SIP"},
                {"u_name", "yb"},
                {"is_sim", false},
                {"df_list", feature_list}
            }
        }};

        vector<string> request_headers = {"Content-Type: application/json"};
        string request_body = params.dump();
        string url =  "http://" + iottalk_server + ":" + iottalk_server_port + "/" + mac_addr;

        string response = requests::post(
            url, 
            request_headers,
            request_body
        );

        return response;
    }

    //void push(string data_str, string feature = default_input_feature){
    void push(std::vector<string> &data, string feature = default_input_feature){    
        //vector<string> data = {data_str};
        vector<string> request_headers = {"Content-Type: application/json"};
        
        string request_body = json({{"data",data}}).dump();

        string url =  "http://" + iottalk_server + ":" + iottalk_server_port + "/" + mac_addr + "/" + feature;
        string response = requests::put(url,
            request_headers,
            request_body
        );
    }

    void push_twin(string data_str1, string data_str2, string feature = default_input_feature){
        vector<string> data = {data_str1, data_str2};
        vector<string> request_headers = {"Content-Type: application/json"};
        
        string request_body = json({{"data",data}}).dump();

        string url =  "http://" + iottalk_server + ":" + iottalk_server_port + "/" + mac_addr + "/" + feature;
        string response = requests::put(url,
            request_headers,
            request_body
        );
    }

    string pull(string feature = default_output_feature){
        int retry_cnt = 0;
        json content;
        string retStr = "empty";
        string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + mac_addr + "/" + feature;
        
        while (true){
            string response = requests::get(url);
            content = json::parse(response);

            if(content["samples"].size() != 0 || retry_cnt > max_retry) break;
            retry_cnt++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (content["samples"].size() != 0){
            json data = content["samples"][0][1]; // 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
            retStr = data[0].dump();
        }
        
        return retStr;
    }

    vector<string> pull_twin(string feature = default_output_feature){
        int retry_cnt = 0;
        json content;
        vector<string> retArr = {};
        string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + mac_addr + "/" + feature;
        
        while (true){
            string response = requests::get(url);
            content = json::parse(response);

            if(content["samples"].size() != 0 || retry_cnt > max_retry) break;
            retry_cnt++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (content["samples"].size() != 0){
            json data = content["samples"][0][1]; // 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
            for(auto s : data)
                retArr.emplace_back(s);
        }
        
        return retArr;
    }
};

#endif
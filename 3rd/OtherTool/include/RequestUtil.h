#ifndef _RequestUtil_H_
#define _RequestUtil_H_

#include <curl/curl.h>

#include <string>
#include <cstring>
#include <vector>

using namespace std;

class requests{
  public:
  struct MemoryStruct {
    char *memory;
    size_t size;
  };
 
  static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
  {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
  
    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
      /* out of memory! */
      printf("not enough memory (realloc returned NULL)\n");
      return 0;
    }
  
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  
    return realsize;
  }

  static string post(string& url, vector<string>& headers, string& body){
      CURL *curl;
      CURLcode res;
      struct MemoryStruct chunk;
      struct curl_slist *header_list = NULL;
      string response;
      
      chunk.memory = (char*)malloc(1);  /* will be grown as needed by realloc above */
      chunk.size = 0;    /* no data at this point */
      
      curl_global_init(CURL_GLOBAL_ALL);
      curl = curl_easy_init();
      if(curl) {
          for(string s : headers){
              header_list = curl_slist_append(header_list, s.c_str());
          }
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

          curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      
          /* send all data to this function  */
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      
          /* we pass our 'chunk' struct to the callback function */
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
      
          /* some servers do not like requests that are made without a user-agent
          field, so we provide one */
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
          
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
          
          /* Perform the request, res will get the return code */
          res = curl_easy_perform(curl);
          /* Check for errors */
          if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
          }
          else {
          response = chunk.memory;
          }
      
          /* always cleanup */
          curl_easy_cleanup(curl);
      }
      
      free(chunk.memory);
      curl_global_cleanup();
      return response;
  }

  static string put(string& url, vector<string>& headers, string& body){
      CURL *curl;
      CURLcode res;
      struct MemoryStruct chunk;
      struct curl_slist *header_list = NULL;
      string response;
      
      chunk.memory = (char*)malloc(1);  /* will be grown as needed by realloc above */
      chunk.size = 0;    /* no data at this point */
      
      curl_global_init(CURL_GLOBAL_ALL);
      curl = curl_easy_init();
      if(curl) {
          for(string s : headers){
              header_list = curl_slist_append(header_list, s.c_str());
          }
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

          curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      
          /* send all data to this function  */
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      
          /* we pass our 'chunk' struct to the callback function */
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
      
          /* some servers do not like requests that are made without a user-agent
          field, so we provide one */
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
          
          /* Perform the request, res will get the return code */
          res = curl_easy_perform(curl);
          /* Check for errors */
          if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
          }
          else {
            response = chunk.memory;
          }
      
          /* always cleanup */
          curl_easy_cleanup(curl);
      }
      
      free(chunk.memory);
      curl_global_cleanup();
      return response;
  }

  static string get(string& url){
      CURL *curl;
      CURLcode res;
      struct MemoryStruct chunk;
      string response;
      
      chunk.memory = (char*)malloc(1);  /* will be grown as needed by realloc above */
      chunk.size = 0;    /* no data at this point */
      
      curl_global_init(CURL_GLOBAL_ALL);
      curl = curl_easy_init();
      if(curl) {
          curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      
          /* send all data to this function  */
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      
          /* we pass our 'chunk' struct to the callback function */
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
      
          /* some servers do not like requests that are made without a user-agent
          field, so we provide one */
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

          /* Perform the request, res will get the return code */
          res = curl_easy_perform(curl);
          /* Check for errors */
          if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
          }
          else {
          response = chunk.memory;
          }
      
          /* always cleanup */
          curl_easy_cleanup(curl);
      }
      
      free(chunk.memory);
      curl_global_cleanup();
      return response;
  }
};

#endif
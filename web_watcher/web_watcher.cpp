// to compile: g++ -o web_watcher web_watcher.cpp -lcurlpp -lcurl

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <algorithm>
#include <set>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "../cpp_util/cpp_util.hpp"

using namespace curlpp::options;

struct Request
{
    std::string number = "";
    std::string keyword = "";
};

std::set<std::string> string_set;

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append(static_cast<char *>(ptr), size*nmemb);
    return size*nmemb;
}

std::string getSentence(std::string str, size_t pos)
{
    std::string output = "";
    int start = pos;
    while(start>=0)
    {
        if(str.at(start) == ' ')
        {
            break;
        }
        else
        {
            start--;
        }
    }
    int end = pos;
    while(end < str.length())
    {
        if(str.at(end) == ' ')
        {
            break;
        }
        else
        {
            end++;
        }
    }

    //printv(start);
    //printv(end);

    output = str.substr(start+1, (end - start-1));

    return output;
}

std::vector<std::string> getContentForKey(std::string str, std::string keyword)
{
    std::vector<std::string> output;

    size_t keyword_pos = 0;
    while(keyword_pos != std::string::npos)
    {
        keyword_pos = str.find(keyword, keyword_pos);
        if(keyword_pos == std::string::npos)
        {
            return output;
        }
        else
        {
            //printv(keyword_pos);
            std::string sub = getSentence(str, keyword_pos);

            //check if string is a valid address(has http), if not skip it.
            if(sub.find("http", 0) != std::string::npos)
            {
                //printv(sub);
                output.push_back(sub);
            }
            keyword_pos += sub.length();
        }
    }

    return output;
}

void processContents(std::string str, std::vector<Request> requests)
{
    for(int i = 0 ; i < requests.size() ; i++)
    {
        std::vector<std::string> temp_content = getContentForKey(str, requests[i].keyword);
        for(int j = 0 ; j < temp_content.size() ; j++)
        {
            std::string temp_str = temp_content[j];
            if(string_set.find(temp_str) != string_set.end())
            {
                //std::cout<<temp_str<<" is already added!"<<std::endl;
            }
            else
            {
                string_set.insert(temp_str);

                std::string shell_command = "sh ../notification/android_sms.sh " + requests[i].keyword + " " + temp_str;
                printv(shell_command);
                system(shell_command.c_str());
            }
        }
    }
}

std::vector<Request> loadRequests(std::string file_name)
{
    std::vector<Request> output;

    std::vector<std::string> configs = readLinesFromTxt("config.txt");
    for(int i = 0 ; i < configs.size() ; i++)
    {
        std::vector<std::string> tokens = splitString(configs[i], ",");
        //printvec(tokens);
        if(tokens.size() ==2 && tokens[0].length() == 10) //make sure number is in correct format
        {
            std::cout<<"configs["<<i<<"] = "<<configs[i]<<std::endl;
            Request temp_request;
            temp_request.number = tokens[0];
            temp_request.keyword = tokens[1];
            output.push_back(temp_request);
        }
    }

    return output;
}


int main(int, char **)
{
    std::vector<Request> requests;

    CURL *curl;
    CURLcode res;

    while(1)
    {
        std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string time_date = std::ctime(&current_time);
        std::cout<<"\n\n------------------------ "<<time_date;
        requests = loadRequests("config.txt");
        curl = curl_easy_init();

        if(curl) {
            std::string data_payload;
            curl_easy_setopt(curl, CURLOPT_URL, "https://sfbay.craigslist.org/search/sby/zip?");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data_payload);

            res = curl_easy_perform(curl);

            /* always cleanup */
            curl_easy_cleanup(curl);

            //to lower case
            std::transform(data_payload.begin(), data_payload.end(), data_payload.begin(),[](unsigned char c){ return std::tolower(c); });

            //printv(data_payload);

            int string_set_size_prev = string_set.size();
            processContents(data_payload, requests);
            int string_set_size_curr = string_set.size();
            if(string_set_size_prev == string_set_size_curr)
            {
                std::cout<<"No new relavent content found..."<<std::endl;
            }
        }
        else
        {
            std::cout<<"Curl initialization failed..."<<std::endl;
        }

        //sleep(300);
        sleep(1);
    }


    return 0;
}

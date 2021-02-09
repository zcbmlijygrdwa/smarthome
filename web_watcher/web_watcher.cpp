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

std::vector<std::string> getInterestedContents(std::string str, std::vector<std::string> keywords)
{
    std::vector<std::string> output;
    for(int i = 0 ; i < keywords.size() ; i++)
    {
        std::vector<std::string> temp_content = getContentForKey(str, keywords[i]);
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
                output.push_back(temp_str);
            }
        }
    }

    return output;

}


int main(int, char **)
{
    std::vector<std::string> keywords;
    keywords.push_back("pillow");

    CURL *curl;
    CURLcode res;

    while(1)
    {
        curl = curl_easy_init();

        if(curl) {
            std::string data_payload;
            printv(1);
            curl_easy_setopt(curl, CURLOPT_URL, "https://sfbay.craigslist.org/search/sby/zip?");
            printv(2);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            printv(3);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data_payload);
            printv(4);

            res = curl_easy_perform(curl);
            printv(5);

            /* always cleanup */
            curl_easy_cleanup(curl);
            printv(6);

            //to lower case
            std::transform(data_payload.begin(), data_payload.end(), data_payload.begin(),[](unsigned char c){ return std::tolower(c); });

            //printv(data_payload);

            std::vector<std::string> contents = getInterestedContents(data_payload, keywords);

            printv(contents.size());

            if(contents.size()>0)
            {
                //printvec(contents);
                for(int i = 0 ; i < contents.size() ; i++)
                {
                    std::string number = "3128880068";
                    std::string shell_command = "sh ../notification/android_sms.sh " + number + " " + contents[i];
                    printv(shell_command);
                    //system(shell_command.c_str());
                }
            }
            else

            {
                std::cout<<"Cannot find any new content related the keywords..."<<std::endl;
            }

        }
        else
        {
            std::cout<<"Curl initialization failed..."<<std::endl;
        }

        std::cout<<"------------------------"<<std::endl;
        sleep(30);
        //sleep(1);
    }


    return 0;
}

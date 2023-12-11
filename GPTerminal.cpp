#include <iostream>
#include <cstdlib>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include "includes/llm_parameters.hpp"

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response)
{
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

void HelpMessage(char **argv)
{
    if (argv[1] != NULL) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            cout << "This is a ChatBot integrated in your teminal.\n \
                You need an OpenAI API Key in order to use this tool.\n \
                Please set your OpenAI API Key as an env variable.\n \
                Example: export OPENAI_API_KEY=\"sk-XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\"" << endl;
            cout << "If you want to customize the LLM launch the tool like this: \n \
                ./GPTerminal -m gpt-4 -t 0.7 -ctx 280\n \
                -m: Change the model, by default it's 'gpt-3.5-turbo'\n \
                -t: Change the temperature of the LLM (float value from 0 to 1) 0 gives strict responses, 1 gives creative responses. Default is 0.7\n \
                -p: Change the pre-premopt for example 'You will answer to every question like a drunk man of 25 years old'\n \
                -ctx: Change the context size (number of tokens). Basically it's the size of the response. Default is 500."<< endl;
            exit(0);
        }
    }
}

void GetParameters(char **argv, Parameters &llm)
{
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], "-ctx") == 0) {
            llm.ctx = std::stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-t") == 0) {
            llm.temperature = std::stof(argv[i + 1]);
        }
        if (strcmp(argv[i], "-m") == 0) {
            llm.model = argv[i + 1];
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (argv[i + 1] != NULL) {
                llm.system_prompt = argv[i + 1];
                int j = i + 2;
                while (argv[j] != NULL && argv[j][0] != '-') {
                    llm.system_prompt += " ";
                    llm.system_prompt += argv[j];
                    j++;
                }
                break;
            }
        }
        i++;
    };
}

void ChatBot(Parameters llm)
{
    cout << "Enter your message: ";
    while (true) {
        CURL* curl = curl_easy_init();
        getline(cin, llm.user_input);
        if (strcmp(llm.user_input.c_str(), "quit") == 0) {
            exit(0);
        }
        if (curl) {
            json requestData;
            requestData["model"] = llm.model;
            requestData["messages"][0]["role"] = "system";
            requestData["messages"][0]["content"] = llm.system_prompt;
            requestData["messages"][1]["role"] = "user";
            requestData["messages"][1]["content"] = llm.user_input;
            requestData["temperature"] = llm.temperature;
            requestData["max_tokens"] = llm.ctx;

            string requestDataStr = requestData.dump();

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, ("Authorization: Bearer " + llm.OPENAI_API_KEY).c_str());
            curl_easy_setopt(curl, CURLOPT_URL, llm.base_url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            llm.response.clear();

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &llm.response);

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                cerr << "Curl request failed: " << curl_easy_strerror(res) << endl;
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        try {
            json jresponse = json::parse(llm.response);
            cout << GREEN_TEXT << jresponse["choices"][0]["message"]["content"].get<string>() << RESET_COLOR << endl;
        } catch (const json::parse_error& e) {
            cerr << "JSON parsing error: " << e.what() << endl;
        }
    }
}


int main(int argc, char **argv)
{
    Parameters llm;
    HelpMessage(argv);
    if (argc > 1) {
        GetParameters(argv, llm);
    }
    ChatBot(llm);
    return 0;
}

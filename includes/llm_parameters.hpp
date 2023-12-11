#include <iostream>
#include <cstdlib>

#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

class Parameters
{
    public:
        float temperature = 0.9;
        int ctx = 500;
        std::string OPENAI_API_KEY;
        std::string base_url = "https://api.openai.com/v1/chat/completions";
        std::string model = "gpt-3.5-turbo";
        std::string user_input = "Hello !";
        std::string response = "Initial Response"; //Do not change or erase the value.
        std::string system_prompt = "You are a helpful assistant that answer to every questions. Answer with the same language of the question.";

    Parameters() {
        OPENAI_API_KEY = std::getenv("OPENAI_API_KEY");
        if (OPENAI_API_KEY.empty()) {
            std::cout << "Please set the OPENAI_API_KEY env variable (--help).";
            exit(1);
        }
    }
};

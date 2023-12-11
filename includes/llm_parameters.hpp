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
        std::string response = "Initial Response";
        std::string system_prompt = "You are a helpful assistant that answer to every questions. Answer with the same language of the question.";

    Parameters()
    {
        const char *env_key = "OPENAI_API_KEY";
        const char *env_value = std::getenv(env_key);

        if (env_value == nullptr || env_value[0] == '\0')
        {
            std::cerr << "Error: Please set the " << env_key << " environment variable (--help).\n" << std::endl;
            throw std::runtime_error("Missing OPENAI_API_KEY");
        }

        OPENAI_API_KEY = env_value;
    }
};

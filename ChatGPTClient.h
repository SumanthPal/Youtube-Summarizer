//
// Created by Sumanth P on 12/17/23.
//

#ifndef YOUTUBE___CHATGPTCLIENT_H
#define YOUTUBE___CHATGPTCLIENT_H
#include <iostream>
#include <httplib.h>

#include <iostream>
#include <httplib.h>

class ChatGPTClient {
public:
    ChatGPTClient(const std::string& apiKey) : apiKey_(apiKey), client_("api.openai.com") {
        endpoint_ = "/v1/engines/davinci-codex/completions";
        client_.set_default_headers({
                                            {"Content-Type", "application/json"},
                                            {"Authorization", "Bearer " + apiKey_}
                                    });
    }

    std::string getCompletion(const std::string& prompt) {
        std::string data = R"({"prompt": ")" + prompt + R"("})";
        auto response = client_.Post(endpoint_.c_str(), data.c_str(), "application/json");

        if (response && response->status == 200) {
            return response->body;
        } else {
            std::cerr << "Request failed!" << std::endl;
            return ""; // or throw an exception if you prefer
        }
    }

private:
    std::string apiKey_;
    std::string endpoint_;
    httplib::Client client_;
};




#endif //YOUTUBE___CHATGPTCLIENT_H

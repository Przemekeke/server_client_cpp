#include <iostream>
#include <httplib.h>
namespace com_kantarmedia_test {

    bool isNumber(const std::string& str)
    {
         return str.find_first_not_of("0123456789") == std::string::npos;
    }

    class SampleClient {

    public:
        SampleClient() {}
        std::string sort(std::vector<std::string> vec, httplib::Client& cli) {
            std::string input = "/sort?vector=";
            for (auto& s : vec)
                input += s + ",";
            input = input.substr(0, input.size() - 1);
            auto res = cli.Get(input.c_str());
            handleResponse(res);
            return res->body;
        };

        std::string reverse(std::vector<std::string> vec, httplib::Client& cli) {
            std::string input = "/reverse?vector=";
            for (auto& s : vec)
                input += s + ",";
            input = input.substr(0, input.size() - 1);
            auto res = cli.Get(input.c_str());
            handleResponse(res);
            return res->body;
        };

        void handleResponse(httplib::Result& res) {
            if (res->status == 200) {
                std::cout << res->body << std::endl;
            }
            else {
                auto err = res.error();
                std::cout << "ERROR in response: " << err;
            }
        }

    };

}


int main(int argc, char* argv[])
{
    httplib::Client cli("http://localhost:1234");
    com_kantarmedia_test::SampleClient client;
    if (argc < 3) {
        std::cout << "not enough arguments \nUsage: client {sort|reverse} [int0 int1 ... intn]";
 
    }
    else {
        std::string sArg = argv[1];
        if (sArg == "sort") {
            std::vector<std::string> vec;
            for (int i = 2; i < argc; i++) {
                std::string tArg(argv[i]);
                if (com_kantarmedia_test::isNumber(tArg)) {
                    vec.push_back(tArg);
                }
                else {
                    std::cout << "One or more arguments are not number(s). Please try again\n";
                    return 0;
                }
            }
            client.sort(vec, cli);
        }
        else if (sArg == "reverse") {
            std::vector<std::string> vec;
            for (int i = 2; i < argc; i++) {
                std::string tArg(argv[i]);
                if (com_kantarmedia_test::isNumber(tArg)) {
                    vec.push_back(tArg);
                }
                else {
                    std::cout << "One or more arguments are not number(s). Please try again\n";
                    return 0;
                }
            }
            client.reverse(vec, cli);
        }
        return 0;
    }
}

// HTTP_client_server_kantar_task.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <vector>
#include <httplib.h>
#include <string>

namespace com_kantarmedia_test {


    using namespace httplib;

    std::vector<int> split_to_int(std::string s, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<int> res;
        //std::vector<int> res_int;
        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(std::stoi(token));
        }

        res.push_back(std::stoi(s.substr(pos_start)));
        return res;
    }

    void logger(const httplib::Request& req, httplib::Response res) {
        std::cout << "Sending response for client...\nHEADERS:\n";
        for (auto& h : res.headers) {
            std::cout << h.first << " " << h.second << "\n";
        }
        std::cout << "BODY: " << res.body;
        std::cout << "\nSUCCESS";
    }

    class SimpleServer {
        Server srv;
    public:

        SimpleServer() {
            srv.set_logger([](const auto& req, const auto& res) {
                logger(req, res);
            });

            srv.Get("/alive", [&](const Request& req, Response& res) {
                res.set_content("Hello I am server and I am alive", "text/plain");
            });

            srv.Get("/close", [&](const Request& req, Response& res) {

                res.set_content("Hello I am server and I am closing", "text/plain");
            });

            srv.Get("/sort", [&](const Request& req, Response& res) {
                std::string tmp_res = "";
                if (req.has_param("vector")) {
                    auto val = req.get_param_value("vector");
                    auto values = split_to_int(val, ",");

                    std::sort(values.begin(), values.end());
                    for (auto& s : values) {
                        tmp_res += std::to_string(s) + " ";
                    }
                    tmp_res = tmp_res.substr(0, tmp_res.size() - 1);
                }
                res.set_content(tmp_res, "text/plain");
            });


            srv.Get("/reverse", [&](const Request& req, Response& res) {
                std::string tmp_res = "";
                if (req.has_param("vector")) {
                    auto val = req.get_param_value("vector");
                    auto values = split_to_int(val, ",");

                    std::reverse(values.begin(), values.end());
                    for (auto& s : values) {
                        tmp_res += std::to_string(s) + " ";
                    }
                    tmp_res = tmp_res.substr(0, tmp_res.size() - 1);
                }
                res.set_content(tmp_res, "text/plain");
            });

            srv.set_error_handler([](const auto& req, auto& res) {
                auto fmt = "Error Status: %d";
                char buf[BUFSIZ];
                snprintf(buf, sizeof(buf), fmt, res.status);
                res.set_content(buf, "text/plain");
            });

            srv.set_exception_handler([](const auto& req, auto& res, std::exception& e) {
                res.status = 500;
                auto fmt = "Error 500%s";
                char buf[BUFSIZ];
                snprintf(buf, sizeof(buf), fmt, e.what());
                res.set_content(buf, "text/plain");
            });

            srv.new_task_queue = [] { return new ThreadPool(12); };

        };
        void runServer() {
            std::cout << "Server is listening on localhost (127.0.0.1) on the port 1234\n";
            srv.listen("localhost", 1234);
        }
        void stopServer() {
            srv.Get("/stop", [&](const Request& req, Response& res) {
                srv.stop();
            });
        }

    };
}

int main()
{
    com_kantarmedia_test::SimpleServer server;
    server.runServer();
    return 0;
}


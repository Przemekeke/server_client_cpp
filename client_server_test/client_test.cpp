#include "pch.h"
#include "CppUnitTest.h"
#include "../client/client.cpp"
#include "../HTTP_client_server_kantar_task/httplib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace clientTest
{
	TEST_CLASS(clientTest)
	{
	public:
		TEST_METHOD(sort)
		{
			httplib::Client cli("http://localhost:1234");
			com_kantarmedia_test::SampleClient client;
			std::vector<std::string> testVec = { "2", "1", "73", "5"};
			std::string expectVec = "1 2 5 73";
			Assert::AreEqual(expectVec, client.sort(testVec, cli));
		}

		TEST_METHOD(reverse)
		{
			httplib::Client cli("http://localhost:1234");
			com_kantarmedia_test::SampleClient client;
			std::vector<std::string> testVec = { "2", "1", "73", "5"};
			std::string expectVec = "5 73 1 2";
			Assert::AreEqual(expectVec, client.reverse(testVec, cli));
		}
		TEST_METHOD(alive)
		{
			httplib::Client cli("http://localhost:1234");
			auto res = cli.Get("/alive");
		}
		TEST_METHOD(connection_keep_allive) {
			httplib::Client cli("http://localhost:1234");
			cli.set_keep_alive(true);
			cli.Get("/alive");
			cli.set_keep_alive(false);
			cli.Get("/alive");
		}
	};
}

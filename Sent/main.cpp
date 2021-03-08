#include "TcpServer.h"
#include "json.hpp"
//#include "Message.h"
#include "User.h"
#include "tiny_rsa.h"

#include "spdlog/spdlog.h"
#include "config.h"

#include <iostream>
#include <map>

using namespace std;

//std::map<int, std::string> msg_db;
//int msgCount = 0;

template<typename T, std::size_t N, std::size_t... I>
constexpr std::array<T, N + 1>
append_aux(std::array<T, N> a, T t, std::index_sequence<I...>) {
    return std::array<T, N + 1>{a[I]..., t};
}

template<typename T, std::size_t N>
constexpr std::array<T, N + 1> append(std::array<T, N> a, T t) {
    return append_aux(a, t, std::make_index_sequence<N>());
}

template<class Container>
static void split(const std::string &str, Container &cont,
                  char delim = ' ') {
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

User *initUser(TcpServer::Client client) {
    //Read data from connection
    int size = 0;
    while (!(size = client.loadData()));

    char buf[2048];
    strcpy(buf, client.getData());
    std::vector<std::string> words;
    split(buf, words, ':');

    if (words.size() < 2 || words[0] != "iam") {
        return nullptr;
    }

    return new User(basic_string(words[1]));
    /*
    //Read data from connection

    info := strings.Split(data, ":")
    if len(info) < 2 || info[0] != "iam" {
        return nil
    }

    return &User{Username: info[1]}*/
}

//void sendMessage(char buffer[4096]) {
//    try {
//        msg_db[++msgCount] = buffer;
//        std::string str = msg_db[msgCount];
//        msg_db[msgCount] = str.substr(str.find('{'));
//    } catch (const std::exception &e) {
//        std::cerr << e.what();
//    }
//}

bool isPrime(int number) {
    for (int a = 2; a < number; a++) {
        if (number % a == 0) {
            return false;
        }
    }
    return true;
}

#include "spdlog/sinks/basic_file_sink.h"

void basic_logfile_example() {
    try {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/log.txt");
    }
    catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

void initLog(){
    spdlog::set_pattern("[%H:%M:%S %z] [thread %t/%l] %v");
    spdlog::info("Starting Sent server version {}", PROJECT_VERSION);
    basic_logfile_example();
}

void initRSA(){
    srand(time(nullptr));

    int foo[2];
    for (int &i : foo) {
        int random = rand() % 65535;
        for (int currentNum = random;; currentNum--)
            if (isPrime(currentNum)) {
                i = currentNum;
                break;
            }
    }

    auto key_pair = tiny_rsa::gen_keys(foo[0], foo[1]);
    spdlog::info("OPEN KEY  : {0:x} {1:x}", key_pair.first.e, key_pair.first.n);
    spdlog::info("SECRET KEY: {0:x} {1:x}", key_pair.second.e, key_pair.second.n);

    vector<uint8_t> ret = tiny_rsa::encrypt("Test string for encrypt and decrypt", key_pair.first);
    spdlog::info("Test string for encrypt -> {}", tiny_rsa::vecToString(ret));

    vector<uint8_t> ret2 = tiny_rsa::decrypt(ret, key_pair.second);
    spdlog::info("{} -> {}", tiny_rsa::vecToString(ret), tiny_rsa::vecToString(ret2));
}

int startServer(TcpServer *server){
    //Запуск серевера
    if (server->startServer() == TcpServer::status::up) {
        //Если сервер запущен вывести сообщение и войти в поток ожиданий клиентов
        spdlog::info("Server is up!");
        server->joinLoop();
    } else {
        //Если сервер не запущен вывод кода ошибки и заверешение программы
        spdlog::error("Server start error! Error code: {}", int(server->getStatus()));
        return -1;
    }

    return 0;
}

void help() {
    cout << "---- tinyRSA\n"
            "start -- Start serve\n"
            "d <exp> <mod> <file in> <file out> -- Decrypt file\n"
            "g -- Generate key pair from default primes\n"
            "G <p> <q> -- Generate key pair from primes\n"
            "h -- Help\n"
            "q -- Exit\n" << endl;
}

int main() {

    initLog();
    initRSA();

    TcpServer server ([](TcpServer::Client client) {
        //Вывод адреса подключившего клиента в консоль
        spdlog::info("Connect host! {}", client.getHost());

        User *user = initUser(client);

        if (user == nullptr) { return 1; } //Если имя пользователя пустое - заверщаем сессию

        spdlog::info("{} joined to chat", user->username);

        //Message msg{user->username, user->username + " has joined\n", false};
        //append(broad, msg);

        for (;;) {
            //Ожидание данных от клиента
            int size = 0;
            while (!(size = client.loadData()));

            if (size == -1)
                continue;
            //Вывод размера данных и самих данных в лог
            spdlog::info("{}==================={}", size, client.getData());

            std::vector<std::string> words;
            split(client.getData(), words, ':');

            if (words.size() < 2 || words[0] != "msg") { continue; }
            //Message msg{user->username, append("Hanriel:" + words[1]), false};
            //append(broad, msg);

            //                         std::string str = client.getData();
            //                         std::cout << str << std::endl << "===================" << std::endl;
            std::string ans = "Hanriel:HTTP/1.0 200 OK\r";
            //
            //                         //Отправка ответа клиенту
            //                         //const char answer[1024];
            char *tab2 = new char[ans.length() + 1];
            std::strcpy(tab2, ans.c_str());
            client.sendData(tab2, sizeof(tab2));

            if("bye:00000000" == client.getData()) break;
        }
        return 0;
    });
    startServer(&server);

    while (true) {
        char cmd;
        cin >> cmd;
        if (cmd == 'q') {
            cout << "Bye." << endl;
            break;
        } else if (cmd == 'r') {
            server.restartServer();
        } else if (cmd == 's') {
            server.stopServer();
        } else if (cmd == 'h')
            help();
    }

    return 0;
}

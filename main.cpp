#include "TcpServer.h"
#include "json.hpp"

#include <iostream>
#include <map>

std::map<int, std::string> msg_db;
int msgCount = 0;

void sendMessage(char buffer[4096]){
    try {
        msg_db[++msgCount] = buffer;
        std::string str = msg_db[msgCount];
        msg_db[msgCount] = str.substr(str.find('{'));
    } catch (const std::exception& e){
        std::cerr << e.what();
    }

}

int main() {
    TcpServer server(25555,

                     [](TcpServer::Client client) {

                         //Вывод адреса подключившего клиента в консоль
                         std::cout << "Connect host!" << std::endl;
                         //Ожидание данных от клиента
                         int size = 0;
                         while (!(size = client.loadData()));

                         //Вывод размера данных и самих данных в консоль
                         std::cout
                                 << "size: " << size << " bytes" << std::endl
                                 << "===================" << std::endl
                                 << client.getData() << std::endl
                                 << "===================" << std::endl;

                         std::string str = client.getData();

                         if(str.find('{') < str.size()){
                             str = str.substr(str.find('{'));

                             std::cout << str <<  std::endl << "===================" << std::endl;

                             auto j3 = nlohmann::json::parse(str);

                             std::cout << j3["msg"] <<  std::endl << "===================" << std::endl;

                             std::string ans = "Success: HTTP/1.0 200 OK\r\n" + j3["msg"].get<std::string>();

                             //Отправка ответа клиенту
                             //const char answer[1024];
                             char * tab2 = new char [ans.length()+1];

                             std::strcpy(tab2, ans.c_str());

                             client.sendData(tab2, sizeof(tab2));
                         } else {
                             const char answer[] = "Bad Request: HTTP/1.0 400 Bad Request\r\n";
                             client.sendData(answer, sizeof(answer));
                         }


                     }

    );

    //Запуск серевера
    if (server.startServer() == TcpServer::status::up) {
        //Если сервер запущен вывести сообщение и войти в поток ожиданий клиентов
        std::cout << "Server is up!" << std::endl;
        server.joinLoop();
    } else {
        //Если сервер не запущен вывод кода ошибки и заверешение программы
        std::cout << "Server start error! Error code:" << int(server.getStatus()) << std::endl;
        return -1;
    }
}

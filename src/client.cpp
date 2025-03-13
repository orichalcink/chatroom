#include <SFML/Network.hpp>
#include "io.hpp"
#include <atomic>
#include <mutex>
#include <thread>

using Port = int;

std::atomic<bool> quit = false;
std::mutex mutex;

void send_messages(sf::TcpSocket& socket, const std::string& username);
void receive_messages(sf::TcpSocket& socket);

int main()
{
   srand(time(nullptr));

   Port port = get_integer_input("Input desired server's port: ");

   sf::TcpSocket socket;
   if (socket.connect("127.0.0.1", port) != sf::Socket::Status::Done)
      throw std::runtime_error("Could not connect to the given port!");

   socket.setBlocking(false);
   std::cout << "Connected to server!\n";

   std::string username = get_valid_username("Please input your username: ");
   username = colors[rand() % color_count] + username + res;

   sf::Packet startingPacket;
   startingPacket << username + " connected.";

   if (socket.send(startingPacket) != sf::Socket::Status::Done)
   {
      socket.disconnect();
      throw std::runtime_error("Could not send data!");
   }

   std::thread send(send_messages, std::ref(socket), username);
   std::thread receive(receive_messages, std::ref(socket));

   if (send.joinable())
      send.join();

   if (receive.joinable())
      receive.join();

   sf::Packet disconnectPacket;
   disconnectPacket << username + " disconnected.";
   socket.send(disconnectPacket);

   std::cout << "Disconnected." << std::endl;
   socket.disconnect();
}

void send_messages(sf::TcpSocket& socket, const std::string& username)
{
   while (true)
   {
      std::cout << "Input your message ('exit' to exit): ";

      std::string message;
      std::getline(std::cin >> std::ws, message);
      clear_line();

      if (message == "exit")
      {
         quit = true;
         std::cout << "Exiting..." << std::endl;
         return;
      }

      std::string full_message = username + ": " + message;
      std::cout << full_message << std::endl;

      sf::Packet packet;
      packet << full_message;

      std::lock_guard<std::mutex> lock (mutex);

      if (socket.send(packet) != sf::Socket::Status::Done)
      {
         socket.disconnect();
         throw std::runtime_error("Could not send data!");
      }
   }
}

void receive_messages(sf::TcpSocket& socket)
{
   while (true)
   {
      if (quit)
         return;

      sf::Packet received;

      std::lock_guard<std::mutex> lock (mutex);

      if (socket.receive(received) == sf::Socket::Status::Done)
      {
         std::string message;
         received >> message;

         clear_line(0);
         std::cout << message << std::endl;
         std::cout << "Input your message ('exit' to exit): " << std::flush;
      }
   }
}

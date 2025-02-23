#include <SFML/Network.hpp>
#include "io.hpp"
#include <atomic>
#include <mutex>
#include <thread>

std::atomic<bool> quit = false;
std::mutex mutex;

void sendMessages(sf::TcpSocket& socket, const std::string& username);
void receiveMessages(sf::TcpSocket& socket);

int main()
{
   srand(time(nullptr));

   int port = getInteger("Input desired server's port: "s);

   sf::TcpSocket socket;
   if (socket.connect("127.0.0.1", port) != sf::Socket::Status::Done)
      throw std::runtime_error("Could not connect!");

   socket.setBlocking(false);
   std::cout << "Connected to server!\n";
   std::cout << "Please input your username: ";

   std::string username;
   std::getline(std::cin >> std::ws, username);

   while (username.size() > 24 || username.size() < 3)
   {
      clearln();
      std::cout << red << "Invalid username, try again: " << res;
      std::getline(std::cin >> std::ws, username);
   }

   username = colors[rand() % color_count] + username + res;

   sf::Packet startingPacket;
   startingPacket << username + " connected.";

   if (socket.send(startingPacket) != sf::Socket::Status::Done)
   {
      socket.disconnect();
      throw std::runtime_error("Could not send data!");
   }

   std::thread send(sendMessages, std::ref(socket), username);
   std::thread receive(receiveMessages, std::ref(socket));

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

void sendMessages(sf::TcpSocket& socket, const std::string& username)
{
   while (true)
   {
      std::cout << "Input your message ('exit' to exit): ";

      std::string message;
      std::getline(std::cin >> std::ws, message);
      clearln();

      if (message == "exit")
      {
         quit = true;
         std::cout << "Exiting..." << std::endl;
         return;
      }

      std::cout << username << ": " << message << std::endl;

      sf::Packet packet;
      packet << username + ": " + message;

      std::lock_guard<std::mutex> lock (mutex);

      if (socket.send(packet) != sf::Socket::Status::Done)
      {
         socket.disconnect();
         throw std::runtime_error("Could not send data!");
      }
   }
}

void receiveMessages(sf::TcpSocket& socket)
{
   while (true)
   {
      if (quit) return;

      sf::Packet received;

      std::lock_guard<std::mutex> lock (mutex);

      if (socket.receive(received) == sf::Socket::Status::Done)
      {
         std::string message;
         received >> message;

         clearln(0);
         std::cout << message << std::endl;
         std::cout << "Input your message ('exit' to exit): " << std::flush;
      }
   }
}

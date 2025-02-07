#include <SFML/Network.hpp>
#include "io.hpp"
#include <memory>

int main()
{
   std::vector<std::unique_ptr<sf::TcpSocket>> clients;

   int port = getInteger("Input server port: "s);

   sf::TcpListener listener;
   if (listener.listen(port, sf::IpAddress::Any) != sf::Socket::Status::Done)
      throw std::runtime_error("Could not connect listener!");

   std::cout << "Waiting for clients to connect...\n";

   sf::SocketSelector selector;
   selector.add(listener);

   while (true)
   {
      if (selector.wait())
      {
         if (selector.isReady(listener))
         {
            auto socket = std::make_unique<sf::TcpSocket>();
            if (listener.accept(*socket) != sf::Socket::Status::Done)
               throw std::runtime_error("Could not accept socket!");

            selector.add(*socket);
            clients.push_back(std::move(socket));
         }

         for (auto it = clients.begin(); it != clients.end();)
         {
            if (selector.isReady(**it))
            {
               sf::Packet packet;

               if ((*it)->receive(packet) == sf::Socket::Done)
               {
                  std::string data;
                  packet >> data;
                  std::cout << data << std::endl;

                  sf::Packet echoPacket;
                  echoPacket << data;

                  for (auto& socket : clients)
                     if (socket.get() != it->get())
                        if (socket->send(echoPacket) != sf::Socket::Status::Done)
                           std::cout << "Failed to echo user's message!" << std::endl; 

                  ++it;
               }
               else
               {
                  std::cout << "Client disconnected." << std::endl;
                  selector.remove(**it);
                  it = clients.erase(it);
               }
            }
            else ++it;
         }
      }
   }

   std::cout << "Server disconnected." << std::endl;

   for (auto& socket : clients)
      socket->disconnect();
}
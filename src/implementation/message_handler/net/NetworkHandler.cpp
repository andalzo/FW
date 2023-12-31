#include "NetworkHandler.h"
#include <iostream>

namespace FW
{

    void NetworkHandler::SetInfrastructure(const std::string& re)
    {
        this->raw_endpoint = re;
        ParseRawEndpoint();
    }

    void NetworkHandler::ParseRawEndpoint()
    {
        this->infrastructure.reset();
        std::vector<std::string> strings = StringUtility::custom_split(raw_endpoint, ':');
        if(strings.size() == 3 || strings.size() == 2)
        {
            if(strings.at(0) == "serial" || strings.at(0) == "udp" || strings.at(0) == "tcp")
            {
                if(strings.at(0) == "serial")
                {
                    this->infrastructure->type = InfrastructureType::Serial;
                    this->infrastructure->target = strings.at(1);
                    this->infrastructure->port = -1;
                    return;
                }
                if(strings.at(0) == "udp")
                {
                    Infrastructure inf;
                    inf.type = InfrastructureType::UDP;
                    inf.target = strings.at(1);
                    inf.port = std::stoi(strings.at(2));
                    infrastructure = inf;
                    return;
                }
                if(strings.at(0) == "tcp")
                {
                    this->infrastructure->type = InfrastructureType::TCP;
                    this->infrastructure->target = strings.at(1);
                    this->infrastructure->port = std::stoi(strings.at(2));
                    return;
                }
            }
        }
        std::cout << "[FW-LOG-ERROR] : The provided end point string could not be parsed. " << std::endl;
    }

	void NetworkHandler::Send(const mavlink_message_t &msg)
	{
		if (generic_socket->IsOpen())
		{
			generic_socket->Send(msg);
		}
	}

	TSQueue<mavlink_message_t> &NetworkHandler::Incoming()
	{
		return generic_socket->Incoming();
	}

	bool NetworkHandler::IsOpen() const
	{
		return generic_socket->IsOpen();
	}

	std::string NetworkHandler::GetEndPoint() const
	{
		return raw_endpoint;
	}

	TSQueue<mavlink_message_t> &NetworkHandler::IncomingACK()
	{
		return generic_socket->IncomingAck();
	}

    void NetworkHandler::Start()
    {
        if(!infrastructure.has_value())
        {
            std::cout << "[FW-LOG-ERROR]: Must provide a end point string before starting network handler. " << std::endl;
            return;
        }
        if(generic_socket != nullptr)
        {
            std::cout << "[FW-LOG-ERROR]: The generic socket in network handler already started." << std::endl;
            return;
        }

        switch (this->infrastructure->type)
        {
            case UDP:
            {
                generic_socket = std::make_shared<UDPSocket>(this->infrastructure->target, this->infrastructure->port);
                break;
            }

            case TCP:
            {
                // TODO: Implement TCP (on top of UDP it seems)
                break;
            }

            case Serial:
            {
                // TODO: Implement Serial
                break;
            }
            default:
            {
                std::cout << "[FW-LOG-ERROR]:The unknown infrastructure type! " << std::endl;
            }
        }

        if(!generic_socket->Start())
        {
            std::cout << "[FW-LOG-ERROR]: The generic socket in network handler couldn't start." << std::endl;
        }

        reader_thread = std::thread([this](){generic_socket->ReaderThread();});
        writer_thread = std::thread([this](){generic_socket->WriterThread();});
        std::this_thread::sleep_for(500ms);
    }

    void NetworkHandler::Close()
    {
	   if (generic_socket == nullptr)
	   {
		   std::cout << "[FW-LOG-ERROR]: The generic socket was already closed." << std::endl;
		   return;
	   }

       generic_socket->Close();
        if(writer_thread.joinable())
        {
            writer_thread.join();
            std::cout << "[FW-LOG-DEBUG]: Writer thread joined." << std::endl;
        }
        if(reader_thread.joinable())
       {
          reader_thread.join();
          std::cout << "[FW-LOG-DEBUG]: Reader thread joined." << std::endl;
       }
	   generic_socket = nullptr;
    }

} // FW











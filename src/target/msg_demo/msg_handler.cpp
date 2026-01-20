#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cppzmq/zmq.hpp>
#include <google/protobuf/any.pb.h>

class ZMQSubscriber {
 public:
  ZMQSubscriber() : context_(1), socket_(ZMQ_SUBSCRIBER, "tcp://localhost:5555") {
    socket_.connect("tcp://localhost:5555");
  }

  void LoadMessages() {
    std::vector<std::string> messages;
    messages.reserve(100);

    zmq::message_t message;
    while (socket_.recv(&message, zmq::recv_flags::dontwait) == 0) {
      messages.push_back(std::string(static_cast<char*>(message.data()), message.size()));
      message.rebuild();
    }

    for (const auto& message_str : messages) {
      std::unique_ptr<google::protobuf::Message> message(google::protobuf::Message::ParseFromString(message_str));
      if (message != nullptr) {
        // Do something with the message
        std::cout << "Received message: " << message->DebugString() << std::endl;
        // Deserialize the message into a protobuf Any type
        google::protobuf::Any any_message;
        if (any_message.ParseFromString(message_str)) {
          // Do something with the Any message
          std::cout << "Received Any message: " << any_message.GetTypeUrl() << std::endl;
        }
      }
    }
  }

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
};
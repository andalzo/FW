add_library(FW
        STATIC
        #Interface Files
        interface/FWContext.cpp
        #Implementation Files
        implementation/FWImpl.cpp
        implementation/message_handler/net/NetworkHandler.cpp
        implementation/message_handler/net/infrastructure/SerialSocket.cpp
        implementation/message_handler/net/infrastructure/UDPSocket.cpp
        implementation/utility/StringUtility.cpp
        implementation/message_handler/mavlink_handler/mavlink_subscriber/MavlinkSubscriber.cpp
        implementation/message_handler/mavlink_handler/mavlink_commander/MavlinkCommander.cpp
        implementation/message_handler/MessageHandler.cpp implementation/message_handler/net/GenericSocket.cpp
        implementation/message_handler/mavlink_handler/mavlink_subscriber/Subscription.cpp
        implementation/message_handler/mavlink_handler/TimeoutDetector.cpp
        implementation/System.h
)


if(WIN32)
    message("Detected WIndowsSDK. So target linked with wsock32 and ws2_32")
    target_link_libraries(FW PUBLIC wsock32 ws2_32)
endif()


target_include_directories(FW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/interface)
target_include_directories(FW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/implementation)
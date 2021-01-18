cmake_minimum_required(VERSION 3.17.3)
project(Advance-Programming)


set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_C_STANDARD 99)
#set(CMAKE_CXX_FLAGS -O2)
#add_executable(Advance-Programming main.cpp)

#adam ass1
#add_executable(Advance-Programming ass1-adam/MainTrain.cpp ass1-adam/anomaly_detection_util.cpp ass1-adam/anomaly_detection_util.h)

#oz ass1
#add_executable(Advance-Programming ass1-oz/MainTrain.cpp ass1-oz/anomaly_detection_util.h ass1-oz/anomaly_detection_util.cpp)

#add_executable(Advance-Programming
#        ass2/anomaly_detection_util.cpp
#        ass2/anomaly_detection_util.h
#        ass2/mainTrain.cpp
#        ass2/SimpleAnomalyDetector.cpp
#        ass2/SimpleAnomalyDetector.h
#        ass2/timeseries.cpp
#        ass2/timeseries.h
#        ass2/AnomalyDetector.h)
#

#add_executable(Advance-Programming ass3/mainTrain.cpp ass3/minCircle.h)

#add_executable(Advance-Programming ass4/IO/DefaultIO.h ass4/IO/StandartIO.cpp ass4/IO/StandartIO.h ass4/Command/Command.h ass4/Command/printCommand.cpp ass4/Command/printCommand.h ass4/Command/CliCommand.cpp ass4/Command/CliCommand.h ass4/CLI.cpp ass4/Command/UploadCommand.cpp ass4/Command/UploadCommand.h ass4/Command/DownloadCommand.cpp ass4/Command/DownloadCommand.h ass4/Command/ExitCommand.cpp ass4/Command/ExitCommand.h ass4/Command/Exitable.h)
#add_executable(Advance-Programming1 serv_test/client.cpp serv_test/client.h)
add_executable(Advance-Programming
        ass4/Anomaly-detection/anomaly_detection_util.cpp
        ass4/Anomaly-detection/anomaly_detection_util.h
        ass4/Anomaly-detection/SimpleAnomalyDetector.cpp
        ass4/Anomaly-detection/SimpleAnomalyDetector.h
        ass4/Anomaly-detection/timeseries.cpp
        ass4/Anomaly-detection/timeseries.h
        ass4/Anomaly-detection/AnomalyDetector.h
        ass4/Anomaly-detection/minCircle.h
        ass4/Anomaly-detection/minCircle.cpp
        ass4/Anomaly-detection/HybridAnomalyDetector.h
        ass4/Anomaly-detection/HybridAnomalyDetector.cpp
        ass4/Command/commands.h
        ass4/Command/MainTrain.cpp
        ass4/Command/CLI.h
        ass4/Command/CLI.cpp
        #        ass2/mainTrain.cpp
        )

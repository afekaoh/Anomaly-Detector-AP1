cmake_minimum_required(VERSION 3.17.3)
project(AdvanceProg1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -O0)
#add_executable(Ad`vanceProg1 main.cpp)

#adam ass1
#add_executable(AdvanceProg1 ass1-adam/MainTrain.cpp ass1-adam/anomaly_detection_util.cpp ass1-adam/anomaly_detection_util.h)

#oz ass1
#add_executable(AdvanceProg1 ass1-oz/MainTrain.cpp ass1-oz/anomaly_detection_util.h ass1-oz/anomaly_detection_util.cpp)

add_executable(AdvanceProg1 ass2/anomaly_detection_util.cpp ass2/anomaly_detection_util.h
        ass2/mainTrain.cpp ass2/SimpleAnomalyDetector.cpp ass2/SimpleAnomalyDetector.h ass2/timeseries.cpp ass2/timeseries.h
        ass2/AnomalyDetector.h)

#add_executable(AdvanceProg1 ass3/mainTrain.cpp ass3/minCircle.h ass3/minCirg4g.h)
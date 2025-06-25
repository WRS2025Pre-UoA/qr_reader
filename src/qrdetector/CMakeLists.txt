cmake_minimum_required(VERSION 3.8)
project(qr_detector)

# 使用するC++のバージョン                                                      
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies                                                            
# find必要な依存パッケージ                                                     
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(zbar REQUIRED)                                                   
#pkg_check_modules(ZBAR REQUIRED zbar)                                         

# zbar を pkg-config で探す                                                    
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZBAR REQUIRED zbar)


include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${ZBAR_INCLUDE_DIRS}
  )

link_directories(${ZBAR_LIBRARY_DIRS})

# 実行ファイルを追加（qr_detector_node.cppをビルド）                                      
add_executable(qr_detector_node src/qr_detector_node.cpp)

# リンクするライブラリ                                                         
ament_target_dependencies(qr_detector_node
  rclcpp
  sensor_msgs
  cv_bridge
  image_transport
)

target_link_libraries(qr_detector_node
  ${OpenCV_LIBRARIES}
  ${ZBAR_LIBRARIES}
)

# インストール設定（ノードとして使えるように）                                 
install(TARGETS
  qr_detector_node
  DESTINATION lib/${PROJECT_NAME}
)

# パッケージをamentで登録                                                      
ament_package()
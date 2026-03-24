# 1. Yêu cầu phiên bản CMake tối thiểu
cmake_minimum_required(VERSION 3.10)

# 2. Khai báo tên Project và ngôn ngữ
project(CodeTayApp VERSION 1.0 LANGUAGES CXX)

# Thiết lập chuẩn C++ (Khuyên dùng C++17 cho các project hiện đại)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Cấu hình SFML
# QUAN TRỌNG: Nếu bạn muốn build dạng Static (đóng gói luôn thư viện vào file .exe)
# thì phải đặt biến này thành TRUE TRƯỚC khi gọi find_package
set(SFML_STATIC_LIBRARIES TRUE) 
find_package(SFML 2 COMPONENTS graphics window system audio network REQUIRED)

# 4. Quét tất cả file mã nguồn (.cpp)
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Khai báo file thực thi
add_executable(CodeTayApp ${SRC_FILES})

# 5. Chỉ định đường dẫn tới các thư mục chứa header (.h)
target_include_directories(CodeTayApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Core
    ${CMAKE_SOURCE_DIR}/include/Screen
    ${CMAKE_SOURCE_DIR}/include/SinglyLinkedList
    ${CMAKE_SOURCE_DIR}/include/UI
    ${CMAKE_SOURCE_DIR}/include/AVLTree
)

# 6. Định nghĩa SFML_STATIC để mã nguồn hiểu là đang dùng bản tĩnh
target_compile_definitions(CodeTayApp PRIVATE SFML_STATIC)

# 7. Liên kết thư viện SFML vào chương trình
# Lưu ý: sfml-system thường phải đặt ở cuối cùng
target_link_libraries(CodeTayApp PRIVATE
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system 
)